#include "Globals.h"
#include "ScriptBindings.h"
#include "Engine/Game/Scripting/Bindings/ScriptBindingsDetail.h"

#include "Engine/Game/World.h"
#include "Engine/Game/Entity.h"
#include "Engine/Game/Components.h"

namespace Sengine::Scripting
{
	void RegisterBindings(sol::state& lua, BindingContext& context)
	{
		Detail::RegisterLogging(lua);
		Detail::RegisterMathTypes(lua);
		Detail::RegisterBaseComponents(lua, context);
		Detail::RegisterPhysicsComponents(lua, context);
		Detail::RegisterEntity(lua, context);
		Detail::RegisterEngineApi(lua, context);
		Detail::RegisterScriptBase(lua, context);
	}
}//namespace Sengine::Scripting

namespace Sengine::Scripting::Detail
{
	void RegisterLogging(sol::state& lua)
	{
		//Route Lua's print through the engine logger so it gets timestamps like everything else.
		lua.set_function("print", [](sol::variadic_args args)
			{
				lua_State* L = args.lua_state();
				std::string line;

				for (auto arg : args)
				{
					if (!line.empty())
					{
						line += '\t';
					}

					//Same conversion Lua's own print uses (respects __tostring).
					std::size_t length = 0;
					const char* text = luaL_tolstring(L, arg.stack_index(), &length);
					line.append(text, length);
					lua_pop(L, 1);
				}

				Logging::Log(Logging::ELogType::Info, "[Lua] " + line);
			});
	}

	void RegisterEngineApi(sol::state& lua, BindingContext& context)
	{
		sol::table engine = lua.create_named_table("engine");

		//First script with this name (file stem: "Player" for Player.lua), or nil.
		//If several entities run the same script, use GetScripts or entity:GetScript().
		engine.set_function("GetScript", [&context](const std::string& name, sol::this_state L) -> sol::object
			{
				for (const auto& [entity, script] : context.Scripts)
				{
					if (script.Name == name)
					{
						return sol::make_object(L, script.Self);
					}
				}
				return sol::make_object(L, sol::lua_nil);
			});

		engine.set_function("GetScripts", [&context](const std::string& name, sol::this_state L)
			{
				sol::state_view state(L);
				sol::table result = state.create_table();

				for (const auto& [entity, script] : context.Scripts)
				{
					if (script.Name == name)
					{
						result.add(script.Self);
					}
				}
				return result;
			});

		engine.set_function("FindEntity", [&context](const std::string& name, sol::this_state L) -> sol::object
			{
				entt::registry& registry = context.GameWorld->GetRegistry();
				for (auto [entity, id] : registry.view<IdentificationComponent>().each())
				{
					if (id.Name == name)
					{
						return sol::make_object(L, Entity(entity, &registry));
					}
				}
				return sol::make_object(L, sol::lua_nil);
			});
	}

	void RegisterScriptBase(sol::state& lua, BindingContext& context)
	{
		//Thin forwards to self.Entity, so scripts can write self:GetComponent(...).
		context.ScriptBase = lua.script(R"(
			local Base = {}
			function Base:GetComponent(type)    return self.Entity:GetComponent(type) end
			function Base:HasComponent(type)    return self.Entity:HasComponent(type) end
			function Base:AddComponent(type)    return self.Entity:AddComponent(type) end
			function Base:RemoveComponent(type) self.Entity:RemoveComponent(type) end
			return Base
		)").get<sol::table>();
	}

	const ComponentBinding& GetComponentBinding(const BindingContext& context, const sol::object& type)
	{
		if (type.get_type() == sol::type::table)
		{
			if (auto it = context.Components.find(type.pointer()); it != context.Components.end())
			{
				return it->second;
			}
		}

		//Thrown errors become Lua errors, so the script author gets a file and line number.
		throw sol::error("expected a component type, e.g. TransformComponent");
	}
}//namespace Sengine::Scripting::Detail