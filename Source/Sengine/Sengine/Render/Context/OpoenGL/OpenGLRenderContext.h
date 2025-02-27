#pragma once

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

		virtual void BeginRender();
		virtual void EndRender();

		virtual void BeginRender2D() override;
		virtual void EndRender2D() override;
	};
}


