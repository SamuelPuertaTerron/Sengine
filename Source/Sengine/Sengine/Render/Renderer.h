/**
 *	Filename: Render.h
 *	Date Created: 21/02/25
 *	Purpose: The base renderer used inside the Client app, contains functionality to render stuff to the screen. Abstracts away graphics APIs like OpenGL. 
 *	Last Modified: 25/02/25
 *	Changes:	   Modified the Existing API to remove the m_RenderContext variable
 ***/

#pragma once

namespace Sengine
{
	class IRenderContext;

	struct Camera2D
	{
		
	};

	struct Camera3D
	{
		
	};

	class Renderer
	{
	public:

		[[nodiscard]] static bool Init(const ERenderContextType& contextType);
		static void Destroy();

		//2D Renderer

		static void BeginRender2D(const Camera2D& camera);
		static void EndRender2D();

		static void Draw2D();

		//3D Renderer
 
		static void BeginRender3D(const Camera3D& camera);
		static void EndRenderer();

	private:
	};
}
