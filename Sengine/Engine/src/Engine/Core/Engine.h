#pragma once
#include "Engine/Core/Window.h"
#include "Engine/Core/ILayer.h"
#include "Engine/Render/Render.h"

namespace Sengine
{
	struct EngineSpecification
	{
		int Width;							
		int Height;
		std::string Title;
		RenderSettings Render;				//Virtual resolution + scaling. Game coordinates live in this space.
	};

	struct EngineContext
	{
		std::unique_ptr<Window> Window;
	};

	namespace Engine
	{
		void CreateAndRun(const EngineSpecification& specification,
			std::vector<std::unique_ptr<ILayer>> layers);

		[[nodiscard]] EngineContext& GetContext();

		void Quit();
	}//namespace Engine
}//namespace Sengine