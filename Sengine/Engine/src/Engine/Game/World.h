#pragma once

#include "entt/entt.hpp"

#include "Engine/Game/ISystem.h"

namespace Sengine
{
	class Entity;

	class World
	{
	public:
		World() = default;
		~World() = default;

		//Registry is not copyable; neither is the World.
		World(const World&) = delete;
		World& operator=(const World&) = delete;

		void OnCreate();
		void OnTick(float deltaTime);
		void OnDestroy();

		Entity CreateEntity(const std::string& name);
		void DestroyEntity(Entity entity);

		//Queues an entity to destroy after the current frame
		void QueueDestroy(Entity entity);

		template<typename TSystem, typename... TArgs>
		TSystem& AddSystem(TArgs&&... args)
		{
			static_assert(std::is_base_of_v<ISystem, TSystem>,
				"TSystem must derive from Sengine::ECS::ISystem");

			auto system = std::make_unique<TSystem>(std::forward<TArgs>(args)...);
			TSystem& ref = *system;
			m_Systems.push_back(std::move(system));

			if (m_bCreated)
			{
				ref.OnCreate(*this); //world already running: create immediately
			}
			return ref;
		}

		void SetSystemEnabled(ISystem& system, bool enabled);

		template<typename TSystem>
		TSystem* GetSystem()
		{
			for (auto& system : m_Systems)
			{
				if (auto* typed = dynamic_cast<TSystem*>(system.get()))
				{
					return typed;
				}
			}
			return nullptr;
		}

		template<typename TSystem>
		void SetSystemEnabled(bool enabled)
		{
			if (TSystem* system = GetSystem<TSystem>())
			{
				SetSystemEnabled(*system, enabled);
			}
		}

		const std::vector<std::unique_ptr<ISystem>>& GetSystems() const { return m_Systems; }

		entt::registry& GetRegistry() { return m_Registry; }
		const entt::registry& GetRegistry() const { return m_Registry; }

	private:
		entt::registry m_Registry;
		std::vector<std::unique_ptr<ISystem>> m_Systems;
		std::vector<entt::entity> m_DestroyQueue;
		bool m_bCreated = false;
	};
}//namespace Sengine
