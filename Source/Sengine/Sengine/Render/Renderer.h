/*
 * How Rendering works is through multiple Begin and End functions
 */

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
