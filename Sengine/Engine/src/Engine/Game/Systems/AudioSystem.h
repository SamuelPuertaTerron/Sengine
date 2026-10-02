#pragma once

#include "Engine/Game/ISystem.h"

namespace Sengine
{
	class AudioSystem : public ISystem
	{
	public:
		void OnCreate(World& world) override;
		void OnTick(World& world, float deltaTime) override;
		void OnDestroy(World& world) override;
		[[nodiscard]] std::string_view GetName() const override { return "Audio System"; }
	};
}//namespace Sengine