#include "Globals.h"
#include "OpenGLRenderContext.h"

namespace Sengine::RendererGL
{
	bool OpenGLRenderContext::Init()
	{
		Logger::Log("Created OpenGL Renderer", ELogType::Info, ELogAreaType::Renderer);

		auto window = Application::GetInstance().GetWindow();
		window->GetNativeWindow()->CreateContext();//use the default options, OpenGL  version 4.6. 

		if (!gladLoadGLLoader((GLADloadproc)GetProcAddress))
		{
			Logger::Log("Failed to load OpenGL functions", ELogType::Error, ELogAreaType::Renderer);
			return false;
		}

		return true;
	}

	void OpenGLRenderContext::Destroy()
	{

	}

	void OpenGLRenderContext::BeginRender()
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(0.25f, 0.6f, 0.75f, 1.0f);
	}

	void OpenGLRenderContext::EndRender()
	{

	}

	void OpenGLRenderContext::BeginRender2D()
	{
		
	}

	void OpenGLRenderContext::EndRender2D()
	{

	}
}
