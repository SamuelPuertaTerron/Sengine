/**
 *	Filename: Render.h
 *	Date Created: 21/02/25
 *	Purpose: The base renderer used inside the Client app, contains functionality to render stuff to the screen. Abstracts away graphics APIs like OpenGL. 
 ***/

#pragma once

namespace Sengine
{
	class Camera;
	class IRenderContext;

	class Renderer
	{
	public:

		[[nodiscard]] static bool Init(const ERenderContextType& contextType);
		static void Destroy();

		//Render
		static void BeginRender(const Camera& camera);
		static void EndRender();

		//2D Renderer

		static void BeginRender2D();
		static void EndRender2D();

		static void Draw2D();

		//3D Renderer
 
		static void BeginRender3D();
		static void EndRenderer();

	private:
		inline static SharedPtr<IRenderContext> m_RenderContext;
	};
}
