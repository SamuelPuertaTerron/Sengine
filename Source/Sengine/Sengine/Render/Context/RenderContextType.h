/**
 *	Filename: RenderContextTypes.h
 *	Date Created: 21/02/25
 *	Purpose: Stores the enum values for graphic apis
 ***/
#pragma once

namespace Sengine
{
	enum class ERenderContextType : uint8_t
	{
		None = 0,
		OpenGL,
		DX11,
		Vulkan,
	};

	namespace Render
	{
		// The graphics API type. changed in the app settings default is OpenGL.  
		inline ERenderContextType g_ERenderConTextType = ERenderContextType::OpenGL;
	}
}