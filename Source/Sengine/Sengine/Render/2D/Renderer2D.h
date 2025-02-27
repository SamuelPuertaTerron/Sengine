#pragma once

namespace Sengine
{
	class IRenderContext;
}

namespace Sengine::Renderer2D
{
	class Renderer2D
	{
	public:
		static bool Init(const std::shared_ptr<IRenderContext>& context);
		static void Destroy();
		 
		static void BeginRender();
		static void EndRender();
		 
		//Draw Functions
		 
		static void DrawQuad();

	private:
		//Just a simple variable to keep track of the begin/end functions calls.
		//Will error if a Draw function is called and either a Begin/End function has not been found. 
		inline static int m_CurrentRenderIndex = 0;

		inline static std::shared_ptr<IRenderContext> m_RenderContext;
	};
}//namespace Sengine::Renderer2D


