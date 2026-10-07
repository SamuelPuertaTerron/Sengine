#pragma once

#include "sol/sol.hpp"
#include "entt/entt.hpp"

namespace Sengine
{
	class World;
}//namespace Sengine

namespace Sengine::Scripting
{
	struct ComponentBinding
	{
		std::string Name;
		std::function<bool(entt::registry&, entt::entity)> Has;
		std::function<sol::object(entt::registry&, entt::entity, lua_State*)> Get;
		std::function<sol::object(entt::registry&, entt::entity, lua_State*)> Add;
		std::function<void(entt::registry&, entt::entity)> Remove;
	};

	struct ScriptHandle
	{
		std::string Name;	//File stem, "Player" for Player.lua.
		sol::table Self;
	};

	struct BindingContext
	{
		World* GameWorld = nullptr;

		std::unordered_map<const void*, ComponentBinding> Components;
		std::unordered_map<entt::entity, ScriptHandle> Scripts;

		sol::table ScriptBase;
	};

	void RegisterBindings(sol::state& lua, BindingContext& context);
}//namespace Sengine::Scripting