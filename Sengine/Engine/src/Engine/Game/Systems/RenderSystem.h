#pragma once

#include "Engine/Game/ISystem.h"

namespace Sengine
{
	class RenderSystem : public ISystem
	{
	public:
		explicit RenderSystem(const Raylib::Camera2D camera)
			: m_Camera(camera) { }

		void OnTick(World& world, float deltaTime) override;
		[[nodiscard]] std::string_view GetName() const override { return "Render System"; }

	private:
		Raylib::Camera2D m_Camera{};
	};
}//namespace Sengine
