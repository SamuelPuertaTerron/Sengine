#include "Globals.h"
#include "World.h"

#include "Engine/Game/Entity.h"
#include "Engine/Game/Components.h"

namespace Sengine
{
	void World::OnCreate()
	{
		m_bCreated = true;
		for (auto& system : m_Systems)
		{
			system->OnCreate(*this);
		}
	}

	void World::OnTick(float deltaTime)
	{
		for (std::size_t i = 0; i < m_Systems.size(); ++i)
		{
			ISystem& system = *m_Systems[i];
			if (system.IsEnabled())
			{
				system.OnTick(*this, deltaTime);
			}
		}


		//Flush deferred destroys once all systems have run this tick.
		for (entt::entity handle : m_DestroyQueue)
		{
			if (m_Registry.valid(handle))
			{
				m_Registry.destroy(handle);
			}
		}

		m_DestroyQueue.clear();
	}

	void World::OnDestroy()
	{
		//Tear down systems in reverse registration order.
		for (auto it = m_Systems.rbegin(); it != m_Systems.rend(); ++it)
		{
			(*it)->OnDestroy(*this);
		}
		m_Systems.clear();

		m_Registry.clear();
		m_bCreated = false;
	}

	Entity World::CreateEntity(const std::string& name)
	{
		Entity entity(m_Registry.create(), &m_Registry);
		entity.AddComponent<IdentificationComponent>(name, true, UUID());
		return entity;
	}

	void World::DestroyEntity(Entity entity)
	{
		if (entity.IsValid())
		{
			m_Registry.destroy(entity.GetHandle());
		}
	}

	void World::QueueDestroy(Entity entity)
	{
		if (entity.IsValid())
		{
			m_DestroyQueue.push_back(entity.GetHandle());
		}
	}
	void World::SetSystemEnabled(ISystem& system, bool enabled)
	{
		if (system.m_bEnabled == enabled)
		{
			return;
		}

		system.m_bEnabled = enabled;

		//Before OnCreate there's nothing to enable or disable yet.
		if (m_bCreated)
		{
			enabled ? system.OnEnable(*this) : system.OnDisable(*this);
		}
	}
}//namespace Sengine
