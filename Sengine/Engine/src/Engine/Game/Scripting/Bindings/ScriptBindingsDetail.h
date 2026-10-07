#pragma once

#include <type_traits>

#include "Engine/Game/Scripting/ScriptBindings.h"

//Internal to the bindings .cpp files; nothing outside them should include this.
//
//The bindings are split across several .cpp files on purpose: sol3 generates a lot of
//code per usertype, and keeping it all in one file exceeds MSVC's per-object-file
//section limit (error C1128). If a file hits the limit again, split it further.

namespace Sengine::Scripting::Detail
{
	//ScriptBindings.cpp
	void RegisterLogging(sol::state& lua);
	void RegisterEngineApi(sol::state& lua, BindingContext& context);
	void RegisterScriptBase(sol::state& lua, BindingContext& context);
	[[nodiscard]] const ComponentBinding& GetComponentBinding(const BindingContext& context, const sol::object& type);

	//MathBindings.cpp
	void RegisterMathTypes(sol::state& lua);

	//ComponentBindings.cpp
	void RegisterBaseComponents(sol::state& lua, BindingContext& context);

	//PhysicsComponentBindings.cpp
	void RegisterPhysicsComponents(sol::state& lua, BindingContext& context);

	//EntityBindings.cpp
	void RegisterEntity(sol::state& lua, BindingContext& context);

	//entt stores no data for empty components (tags like PlaySoundRequestComponent),
	//so there's no object to hand to Lua. For those, Get/Add return true when the
	//tag is present and nil when it isn't.
	template<typename TComponent>
	inline constexpr bool IsTagComponent = std::is_empty_v<TComponent>;

	template<typename TComponent, typename... TMembers>
	void BindComponent(sol::state& lua, BindingContext& context, const std::string& name,
		bool canAddRemove, TMembers&&... members)
	{
		lua.new_usertype<TComponent>(name, sol::no_constructor, std::forward<TMembers>(members)...);

		//The global usertype table doubles as the type id: self:GetComponent(TransformComponent).
		const sol::table typeTable = lua[name];

		ComponentBinding binding;
		binding.Name = name;

		binding.Has = [](entt::registry& r, entt::entity e)
			{
				return r.all_of<TComponent>(e);
			};

		if constexpr (IsTagComponent<TComponent>)
		{
			binding.Get = [](entt::registry& r, entt::entity e, lua_State* L)
				{
					return r.all_of<TComponent>(e) ? sol::make_object(L, true) : sol::make_object(L, sol::lua_nil);
				};
		}
		else
		{
			//A pointer, so Lua edits the real component instead of a copy.
			binding.Get = [](entt::registry& r, entt::entity e, lua_State* L)
				{
					TComponent* component = r.try_get<TComponent>(e);
					return component ? sol::make_object(L, component) : sol::make_object(L, sol::lua_nil);
				};
		}

		if (canAddRemove)
		{
			if constexpr (IsTagComponent<TComponent>)
			{
				//Adding a tag that's already there is a no-op, same as get_or_emplace for data components.
				binding.Add = [](entt::registry& r, entt::entity e, lua_State* L)
					{
						if (!r.all_of<TComponent>(e))
						{
							r.emplace<TComponent>(e);
						}
						return sol::make_object(L, true);
					};
			}
			else
			{
				binding.Add = [](entt::registry& r, entt::entity e, lua_State* L)
					{
						return sol::make_object(L, &r.get_or_emplace<TComponent>(e));
					};
			}

			binding.Remove = [](entt::registry& r, entt::entity e)
				{
					r.remove<TComponent>(e);
				};
		}

		context.Components[typeTable.pointer()] = std::move(binding);
	}
}//namespace Sengine::Scripting::Detail