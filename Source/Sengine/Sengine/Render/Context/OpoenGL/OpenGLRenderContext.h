/**
 *	Filename: OpenGLRenderContext.h
 *	Date Created: 21/02/25
 *	Purpose: The OpenGL renderer
 *	Last Modified: 28/02/25
 *	Changes:	   Removed 2D render context and merged into a single file
 *				   Added Begin and Render functions
 *
 ***/
#pragma once

#include "Render/RenderViewport.h"
#include "Render/Context/IRenderContext.h"

namespace Sengine::RendererGL
{
	class OpenGLRenderContext : public IRenderContext
	{
	public:
		OpenGLRenderContext() = default;
		virtual ~OpenGLRenderContext() override = default;

		virtual bool Init() override;
		virtual void Destroy() override;

		virtual void BeginRender() override;
		virtual void EndRender() override; 

		virtual void BeginRender2D() override;
		virtual void EndRender2D() override;

		virtual void DrawQuad(Vector2 position, Vector2 rotation, Vector2 scale) override;

	private:
		Render::RenderViewport m_Viewport;
	};
}//namespace Sengine::RendererGL