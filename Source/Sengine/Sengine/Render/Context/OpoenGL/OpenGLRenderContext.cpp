#include "Globals.h"
#include "OpenGLRenderContext.h"

namespace Sengine::RendererGL
{
	bool OpenGLRenderContext::Init()
	{
		Logger::Log("Created OpenGL Renderer", ELogType::Info, ELogAreaType::Renderer);

		return true;
	}

	void OpenGLRenderContext::Destroy()
	{

	}

	void OpenGLRenderContext::BeginRender2D()
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(0.25f, 0.6f, 0.75f, 1.0f);
	}

	void OpenGLRenderContext::EndRender2D()
	{

	}
}