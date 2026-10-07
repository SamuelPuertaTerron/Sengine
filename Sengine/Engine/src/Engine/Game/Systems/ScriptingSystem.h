#pragma once

#include "sol/sol.hpp"
#include "entt/entt.hpp"

#include "Engine/Game/ISystem.h"
#include "Engine/Game/Scripting/ScriptBindings.h"

namespace Sengine
{
	class World;

	class ScriptingSystem : public ISystem
	{
	public:
		void OnCreate(World& world) override;
		void OnTick(World& world, float deltaTime) override;
		void OnDestroy(World& world) override;
		[[nodiscard]] std::string_view GetName() const override { return "Script System"; }

	private:
		struct ScriptInstance
		{
			sol::environment Environment;		//Per-entity globals, falls back to the shared globals.
			sol::table Self;					//The table the script returned.

			//Cached hooks; invalid when the script doesn't define them.
			sol::protected_function OnCreate;
			sol::protected_function OnTick;
			sol::protected_function OnDestroy;

			//Collision hooks, called as self:OnCollisionEnter(other).
			sol::protected_function OnCollisionEnter;
			sol::protected_function OnCollisionExit;
			sol::protected_function OnTriggerEnter;
			sol::protected_function OnTriggerExit;

			std::string Name;					//File stem ("Player" for Player.lua). Exposed as self.Name.
			fs::path Path;						//Exposed as self.Path.
			bool bFailed = false;				//Stops a broken script from spamming errors every frame.
		};

		void RegisterBindings();
		ScriptInstance CreateInstance(entt::entity entity, const fs::path& path);
		void BindCollisionHooks(entt::registry& registry, entt::entity entity, const ScriptInstance& instance);
		void RemoveInstance(entt::entity entity);
		void OnScriptComponentDestroyed(entt::registry& registry, entt::entity entity);

		template<typename... TArgs>
		void CallHook(entt::entity entity, sol::protected_function ScriptInstance::* hook,
			std::string_view hookName, TArgs&&... args);

	private:
		//Declared first so it is destroyed last: every sol object below references this state.
		sol::state m_Lua;

		//Lua functions hold references to this; it's cleared in OnDestroy before Lua can run again.
		Scripting::BindingContext m_Bindings;
		std::unordered_map<entt::entity, ScriptInstance> m_Instances;
	};
}//namespace Sengine