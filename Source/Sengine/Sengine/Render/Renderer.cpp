#include "Globals.h"
#include "Renderer.h"

#include "2D/Renderer2D.h"
#include "3D/Renderer3D.h"
#include "Context/IRenderContext.h"

namespace Sengine
{
	bool Renderer::Init(const ERenderContextType& contextType)
	{
		m_RenderContext = IRenderContext::Create(contextType);

		if (!m_RenderContext->Init()) return false;

		Renderer2D::Renderer2D::Init(m_RenderContext);

		return true;
	}

	void Renderer::Destroy()
	{
		Renderer2D::Renderer2D::Destroy();
	}

	void Renderer::BeginRender(const Camera& camera)
	{
		//Update camera matrix

		m_RenderContext->BeginRender();
	}

	void Renderer::EndRender()
	{
		m_RenderContext->EndRender();
	}

	void Renderer::BeginRender2D()
	{
		Renderer2D::Renderer2D::BeginRender();
	}

	void Renderer::EndRender2D()
	{
		Renderer2D::Renderer2D::EndRender();
	}

	void Renderer::Draw2D()
	{

	}
	 
	void Renderer::BeginRender3D()
	{
		
	}

	void Renderer::EndRenderer()
	{

	}
}
