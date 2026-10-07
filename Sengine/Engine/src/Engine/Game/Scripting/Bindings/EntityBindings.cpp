#include "Globals.h"
#include "ScriptBindingsDetail.h"

#include "Engine/Game/World.h"
#include "Engine/Game/Entity.h"
#include "Engine/Game/Components.h"

namespace Sengine::Scripting::Detail
{
	void RegisterEntity(sol::state& lua, BindingContext& context)
	{
		lua.new_usertype<Entity>("Entity",
			sol::no_constructor,

			"IsValid", &Entity::IsValid,

			"GetName", [&context](const Entity& e) -> std::string
			{
				if (!e.IsValid())
				{
					return {};
				}
				const auto* id = context.GameWorld->GetRegistry().try_get<IdentificationComponent>(e.GetHandle());
				return id ? id->Name : std::string{};
			},

			"HasComponent", [&context](const Entity& e, const sol::object& type)
			{
				const ComponentBinding& binding = GetComponentBinding(context, type);
				return e.IsValid() && binding.Has(context.GameWorld->GetRegistry(), e.GetHandle());
			},

			"GetComponent", [&context](const Entity& e, const sol::object& type, sol::this_state L) -> sol::object
			{
				const ComponentBinding& binding = GetComponentBinding(context, type);
				if (!e.IsValid())
				{
					return sol::make_object(L, sol::lua_nil);
				}
				return binding.Get(context.GameWorld->GetRegistry(), e.GetHandle(), L);
			},

			"AddComponent", [&context](const Entity& e, const sol::object& type, sol::this_state L) -> sol::object
			{
				const ComponentBinding& binding = GetComponentBinding(context, type);
				if (!binding.Add)
				{
					throw sol::error(binding.Name + " can't be added from Lua");
				}
				if (!e.IsValid())
				{
					return sol::make_object(L, sol::lua_nil);
				}
				return binding.Add(context.GameWorld->GetRegistry(), e.GetHandle(), L);
			},

			"RemoveComponent", [&context](const Entity& e, const sol::object& type)
			{
				const ComponentBinding& binding = GetComponentBinding(context, type);
				if (!binding.Remove)
				{
					throw sol::error(binding.Name + " can't be removed from Lua");
				}
				if (e.IsValid())
				{
					binding.Remove(context.GameWorld->GetRegistry(), e.GetHandle());
				}
			},

			//The script table on this entity, or nil.
			"GetScript", [&context](const Entity& e, sol::this_state L) -> sol::object
			{
				auto it = context.Scripts.find(e.GetHandle());
				if (it == context.Scripts.end())
				{
					return sol::make_object(L, sol::lua_nil);
				}
				return sol::make_object(L, it->second.Self);
			},

			//Deferred until the end of the frame, so it's safe to call from OnTick.
			"Destroy", [&context](const Entity& e)
			{
				context.GameWorld->QueueDestroy(e);
			},

			sol::meta_function::equal_to, [](const Entity& a, const Entity& b)
			{
				return a.GetHandle() == b.GetHandle();
			},

			sol::meta_function::to_string, [](const Entity& e)
			{
				return std::format("Entity({})", entt::to_integral(e.GetHandle()));
			});
	}
}//namespace Sengine::Scripting::Detail