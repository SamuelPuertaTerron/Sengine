#include "Renderer2D.h"

#include <cassert>

#include "Utils/Assert.h"

namespace Sengine::Renderer2D
{
	void Renderer2D::BeginRender()
	{
		SE_Assert(m_CurrentRenderIndex == 1, "[Render 2D] Error: Has not called End Render function after the draw function");

		m_CurrentRenderIndex++;
	}
	void Renderer2D::EndRender()
	{
		m_CurrentRenderIndex = 0; //Reset the counter
	}
	void Renderer2D::DrawQuad()
	{
		SE_Assert(m_CurrentRenderIndex == 0, "[Render 2D] Error: Has not called either Begin Render function before draw function");
	}
}//namespace Sengine::Renderer2D