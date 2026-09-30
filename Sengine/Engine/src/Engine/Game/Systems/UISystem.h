#pragma once

#include <vector>

#include "entt/entt.hpp"

#include "Engine/Game/ISystem.h"

namespace Sengine
{
	class UISystem : public ISystem
	{
	public:
		void OnCreate(World& world) override;
		void OnTick(World& world, float deltaTime) override;
		void OnDestroy(World& world) override;
		[[nodiscard]] std::string_view GetName() const override { return "UI System"; }

	private:
		void DrawPanels(World& world);
		void DrawText(World& world);
		void DrawButtons(World& world);

	private:
		//Reused every frame to sort panels by layer without reallocating.
		std::vector<entt::entity> m_PanelOrder;
	};
}//namespace Sengine