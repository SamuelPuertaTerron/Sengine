#pragma once

#include "entt/entt.hpp"

namespace Sengine
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, entt::registry* registry)
			: m_Handle(handle), m_Registry(registry) {
		}

		template<typename TComponent, typename... TArgs>
		TComponent& AddComponent(TArgs&&... args)
		{
			return m_Registry->emplace<TComponent>(m_Handle, std::forward<TArgs>(args)...);
		}

		template<typename TComponent>
		TComponent& GetComponent()
		{
			return m_Registry->get<TComponent>(m_Handle);
		}

		template<typename TComponent>
		bool HasComponent() const
		{
			return m_Registry->all_of<TComponent>(m_Handle);
		}

		template<typename TComponent>
		void RemoveComponent()
		{
			m_Registry->remove<TComponent>(m_Handle);
		}

		bool IsValid() const
		{
			return m_Registry != nullptr && m_Registry->valid(m_Handle);
		}

		entt::entity GetHandle() const { return m_Handle; }
		operator entt::entity() const { return m_Handle; }
		explicit operator bool() const { return IsValid(); }

	private:
		entt::entity m_Handle{ entt::null };
		entt::registry* m_Registry = nullptr;
	};
}//namespace Sengine
