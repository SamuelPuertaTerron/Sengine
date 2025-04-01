#include "Globals.h"
#include "OpenGLRenderContext.h"

#include "GLAD/include/glad/glad.h"
#include "Render/Renderer.h"

namespace Sengine::RendererGL
{
	bool OpenGLRenderContext::Init()
	{
		Logger::Log("Created OpenGL Renderer", ELogType::Info, ELogAreaType::Renderer);

		auto window = Application::GetInstance().GetWindow();
		window->GetNativeWindow()->CreateGraphicsContext();//use the default options, OpenGL  version 4.6. 

		if (!gladLoadGLLoader((GLADloadproc)GetProcAddress))
		{
			Logger::Log("Failed to load OpenGL functions", ELogType::Error, ELogAreaType::Renderer);
			return false;
		}

		m_Viewport = Renderer::GetRenderViewport();
		glViewport(m_Viewport.Offset.X, m_Viewport.Offset.Y, m_Viewport.Size.X, m_Viewport.Size.Y);

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

	void OpenGLRenderContext::DrawQuad(Vector2 position, Vector2 rotation, Vector2 scale)
	{
		
	}
}
