#include "Renderer.h"

#include "2D/Renderer2D.h"
#include "3D/Renderer3D.h"

namespace Sengine
{

	void Renderer::BeginRender2D(const Camera2D& camera)
	{
		Renderer2D::Renderer2D::BeginRender();
	}

	void Renderer::EndRender2D()
	{
		Renderer2D::Renderer2D::EndRender();
	}

	void Renderer::Draw2D()
	{
		Renderer2D::Renderer2D::DrawQuad();
	}
	 
	void Renderer::BeginRender3D(const Camera3D& camera)
	{
		
	}

	void Renderer::EndRenderer()
	{

	}
}
