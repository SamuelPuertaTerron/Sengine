#pragma once

#include "Engine/Core/UUID.h"

namespace Sengine
{
	struct IdentificationComponent
	{
		std::string Name{ "Entity" };
		bool IsActive{ true };
		UUID ID{};
	};

	struct TransformComponent
	{
		Raylib::Vector2 Position{ 0.0f, 0.0f };
		Raylib::Vector2 Scale{ 1.0f, 1.0f };
		float Rotation{ 0.0f };
	};
}//namespace Sengine