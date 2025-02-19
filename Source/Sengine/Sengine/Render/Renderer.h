#pragma once

namespace Sengine
{
	struct Camera2D
	{
		
	};

	struct Camera3D
	{
		
	};

	class Renderer
	{
	public:
		//2D Renderer

		static void BeginRender2D(const Camera2D& camera);
		static void EndRender2D();

		static void Draw2D();

		//3D Renderer

		static void BeginRender3D(const Camera3D& camera);
		static void EndRenderer();
	};
}
