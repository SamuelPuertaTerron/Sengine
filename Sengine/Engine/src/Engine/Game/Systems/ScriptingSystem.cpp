#include "Globals.h"
#include "ScriptingSystem.h"

#include "Engine/Game/World.h"
#include "Engine/Game/Entity.h"
#include "Engine/Game/Components.h"

#include "Engine/Game/Scripting/Script.h"

namespace Sengine
{
	namespace
	{
		sol::protected_function GetFunction(const sol::table& self, const char* name)
		{
			sol::object value = self[name];
			if (value.get_type() == sol::type::function)
			{
				return value.as<sol::protected_function>();
			}
			return {};
		}
	}//namespace

	template<typename... TArgs>
	void ScriptingSystem::CallHook(entt::entity entity, sol::protected_function ScriptInstance::* hook,
		std::string_view hookName, TArgs&&... args)
	{
		auto it = m_Instances.find(entity);
		if (it == m_Instances.end() || it->second.bFailed)
		{
			return;
		}

		//Copy what we need: the hook may destroy its own entity/instance while running.
		sol::protected_function function = it->second.*hook;
		if (!function.valid())
		{
			return;
		}

		sol::table self = it->second.Self;
		const std::string name = it->second.Name;

		sol::protected_function_result result = function(self, std::forward<TArgs>(args)...);
		if (!result.valid())
		{
			sol::error err = result;
			Logging::Log(Logging::ELogType::Error, std::format(
				"Script {}: {} failed, disabling it: {}", name, hookName, err.what()));

			if (auto again = m_Instances.find(entity); again != m_Instances.end())
			{
				again->second.bFailed = true;
			}

			//Broken scripts are no longer visible to engine.GetScript / entity:GetScript.
			m_Bindings.Scripts.erase(entity);
		}
	}

	void ScriptingSystem::OnCreate(World& world)
	{
		m_Bindings.GameWorld = &world;

		m_Lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math, sol::lib::string);
		RegisterBindings();

		world.GetRegistry().on_destroy<ScriptComponent>().connect<&ScriptingSystem::OnScriptComponentDestroyed>(this);
	}

	void ScriptingSystem::RegisterBindings()
	{
		Scripting::RegisterBindings(m_Lua, m_Bindings);
	}

	void ScriptingSystem::OnTick(World& world, float deltaTime)
	{
		entt::registry& registry = world.GetRegistry();
		auto view = registry.view<const IdentificationComponent, const ScriptComponent>();

		//Pass 1: create every new instance before running any OnCreate,
		//so engine.GetScript() inside OnCreate can find all of them.
		std::vector<entt::entity> created;
		for (auto [entity, id, script] : view.each())
		{
			if (!script.Script || m_Instances.contains(entity))
			{
				continue;
			}

			ScriptInstance& instance = m_Instances.emplace(entity, CreateInstance(entity, script.Script->GetPath())).first->second;
			if (!instance.bFailed)
			{
				m_Bindings.Scripts[entity] = Scripting::ScriptHandle{ instance.Name, instance.Self };
				BindCollisionHooks(registry, entity, instance);
			}
			created.push_back(entity);
		}

		for (entt::entity entity : created)
		{
			CallHook(entity, &ScriptInstance::OnCreate, "OnCreate");
		}

		//Pass 2: tick.
		for (auto [entity, id, script] : view.each())
		{
			if (id.IsActive)
			{
				CallHook(entity, &ScriptInstance::OnTick, "OnTick", deltaTime);
			}
		}
	}

	void ScriptingSystem::OnDestroy(World& world)
	{
		world.GetRegistry().on_destroy<ScriptComponent>().disconnect<&ScriptingSystem::OnScriptComponentDestroyed>(this);

		std::vector<entt::entity> entities;
		entities.reserve(m_Instances.size());
		for (const auto& [entity, instance] : m_Instances)
		{
			entities.push_back(entity);
		}

		for (entt::entity entity : entities)
		{
			CallHook(entity, &ScriptInstance::OnDestroy, "OnDestroy");
		}

		m_Instances.clear();
		m_Bindings = Scripting::BindingContext{};
		m_Lua.collect_garbage();
	}

	ScriptingSystem::ScriptInstance ScriptingSystem::CreateInstance(entt::entity entity, const fs::path& path)
	{
		ScriptInstance instance;
		instance.Name = path.stem().string();
		instance.Path = path;

		auto fail = [&instance](const std::string& reason)
			{
				Logging::Log(Logging::ELogType::Error, std::format(
					"Script {} cannot be loaded: {}", instance.Name, reason));
				instance.bFailed = true;
				return instance;
			};

		sol::load_result loaded = m_Lua.load_file(path.string());
		if (!loaded.valid())
		{
			sol::error err = loaded;
			return fail(err.what());
		}

		//Own environment per entity, so one script's globals don't leak into another.
		instance.Environment = sol::environment(m_Lua, sol::create, m_Lua.globals());

		sol::protected_function chunk = loaded;
		sol::set_environment(instance.Environment, chunk);

		//Running the file again per entity gives every entity a fresh table.
		sol::protected_function_result result = chunk();
		if (!result.valid())
		{
			sol::error err = result;
			return fail(err.what());
		}

		if (result.get_type() != sol::type::table)
		{
			return fail("the script must 'return' a table");
		}

		instance.Self = result.get<sol::table>();

		//Metadata. These names are reserved on every script table.
		instance.Self["Name"] = instance.Name;
		instance.Self["Path"] = path.generic_string();
		instance.Self["Entity"] = Entity(entity, &m_Bindings.GameWorld->GetRegistry());

		//self:GetComponent etc. A script can override any of them by defining its own.
		for (const auto& [key, value] : m_Bindings.ScriptBase)
		{
			if (instance.Self.raw_get<sol::object>(key).get_type() == sol::type::lua_nil)
			{
				instance.Self.raw_set(key, value);
			}
		}

		instance.OnCreate = GetFunction(instance.Self, "OnCreate");
		instance.OnTick = GetFunction(instance.Self, "OnTick");
		instance.OnDestroy = GetFunction(instance.Self, "OnDestroy");

		instance.OnCollisionEnter = GetFunction(instance.Self, "OnCollisionEnter");
		instance.OnCollisionExit = GetFunction(instance.Self, "OnCollisionExit");
		instance.OnTriggerEnter = GetFunction(instance.Self, "OnTriggerEnter");
		instance.OnTriggerExit = GetFunction(instance.Self, "OnTriggerExit");

		return instance;
	}

	void ScriptingSystem::BindCollisionHooks(entt::registry& registry, entt::entity entity, const ScriptInstance& instance)
	{
		const bool hasAnyHook = instance.OnCollisionEnter.valid() || instance.OnCollisionExit.valid()
			|| instance.OnTriggerEnter.valid() || instance.OnTriggerExit.valid();

		if (!hasAnyHook)
		{
			return;
		}

		auto& callbacks = registry.get_or_emplace<CollisionCallbacksComponent>(entity);

		//Wraps the slot instead of replacing it, so C++ callbacks already set on the
		//entity keep working and run before the Lua hook.
		auto chain = [this, entity](CollisionCallback& slot,
			sol::protected_function ScriptInstance::* hook, std::string_view hookName)
			{
				slot = [this, entity, hook, hookName, previous = std::move(slot)](Entity self, Entity other)
					{
						if (previous)
						{
							previous(self, other);
						}
						CallHook(entity, hook, hookName, other);
					};
			};

		if (instance.OnCollisionEnter.valid()) { chain(callbacks.OnCollisionEnter, &ScriptInstance::OnCollisionEnter, "OnCollisionEnter"); }
		if (instance.OnCollisionExit.valid()) { chain(callbacks.OnCollisionExit, &ScriptInstance::OnCollisionExit, "OnCollisionExit"); }
		if (instance.OnTriggerEnter.valid()) { chain(callbacks.OnTriggerEnter, &ScriptInstance::OnTriggerEnter, "OnTriggerEnter"); }
		if (instance.OnTriggerExit.valid()) { chain(callbacks.OnTriggerExit, &ScriptInstance::OnTriggerExit, "OnTriggerExit"); }
	}

	void ScriptingSystem::RemoveInstance(entt::entity entity)
	{
		m_Bindings.Scripts.erase(entity);
		m_Instances.erase(entity);
	}

	void ScriptingSystem::OnScriptComponentDestroyed(entt::registry& registry, entt::entity entity)
	{
		CallHook(entity, &ScriptInstance::OnDestroy, "OnDestroy");
		RemoveInstance(entity);
	}
}//namespace Sengine