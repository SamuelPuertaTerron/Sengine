#include "Globals.h"
#include "Renderer.h"

#include "2D/Renderer2D.h"
#include "3D/Renderer3D.h"
#include "Context/IRenderContext.h"

namespace Sengine
{
	bool Renderer::Init(const ERenderContextType& contextType)
	{
		auto context = IRenderContext::Create(contextType);

		if (!context->Init()) return false;

		Renderer2D::Renderer2D::Init(context);

		return true;
	}

	void Renderer::Destroy()
	{

	}

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

	}
	 
	void Renderer::BeginRender3D(const Camera3D& camera)
	{
		
	}

	void Renderer::EndRenderer()
	{

	}
}
