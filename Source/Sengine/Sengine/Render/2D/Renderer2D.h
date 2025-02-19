#pragma once

namespace Sengine::Renderer2D
{
	class Renderer2D
	{
	public:
		static void BeginRender();
		static void EndRender();

		//Draw Functions

		static void DrawQuad();

	private:
		//Just a simple variable to keep track of the begin/end functions calls.
		//Will error if a Draw function is called and either a Begin/End function has not been found. 
		inline static int m_CurrentRenderIndex = 0;
	};
}//namespace Sengine::Renderer2D


