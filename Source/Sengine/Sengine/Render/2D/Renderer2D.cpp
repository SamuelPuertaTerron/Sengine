#include "Globals.h"
#include "Renderer2D.h"

#include "Render/Context/IRenderContext.h"

namespace Sengine::Renderer2D
{
	bool Renderer2D::Init(const SharedPtr<IRenderContext>& context)
	{
		m_RenderContext = context;

		return true;
	}

	void Renderer2D::Destroy()
	{
		
	}
	void Renderer2D::BeginRender()
	{
		SE_ASSERT(m_CurrentRenderIndex == 1, "[Render 2D] Error: Has not called End Render function after the draw function");

		m_RenderContext->BeginRender2D();

		m_CurrentRenderIndex++;
	}
	void Renderer2D::EndRender()
	{
		m_RenderContext->EndRender2D();

		m_CurrentRenderIndex = 0; //Reset the counter
	}
	void Renderer2D::DrawQuad()
	{
		SE_ASSERT(m_CurrentRenderIndex == 0, "[Render 2D] Error: Has not called either Begin Render function before draw function");
	}
}//namespace Sengine::Renderer2D