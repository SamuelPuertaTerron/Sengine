#pragma once

namespace Sengine
{
	class World;

	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		virtual void OnCreate(World& world) {}
		virtual void OnTick(World& world, float deltaTime) = 0;
		virtual void OnDestroy(World& world) {}

		//Called when toggled at runtime. Not called at create/destroy.
		virtual void OnEnable(World& world) {}
		virtual void OnDisable(World& world) {}

		[[nodiscard]] virtual std::string_view GetName() const { return "System"; }
		[[nodiscard]] bool IsEnabled() const { return m_bEnabled; }

	private:
		friend class World;
		bool m_bEnabled = true;
	};
}//namespace Sengine
