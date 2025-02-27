/**
 *	Filename: Render2D.h
 *	Date Created: 21/02/25
 *	Purpose: Abstracts away 2D rendering code and used inside the main renderer
 *	Last Modified: 27/02/25
 *	Changes:	   Removed IRenderContext2D from this class and added IRenderContext instead.
 *
 ***/
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


