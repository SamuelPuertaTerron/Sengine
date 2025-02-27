#include "Globals.h"
#include "Application.h"

#include "Window.h"

namespace Sengine
{
	void Application::RunApplication(const std::shared_ptr<ISengineApp>& app)
	{
		m_ClientApp = app;

		if (!Init()) return;

		while (m_Window->GetIsRunning())
		{
			Tick();
		}

		Destroy();
	}

	bool Application::Init()
	{
		if (!m_ClientApp->OnEarlyInit()) return false;

		m_Window = std::make_shared<Window>();

		if (!m_Window->Create(m_ClientApp->GetWindowDescription())) return false;

		if (!Renderer::Init(m_ClientApp->GetWindowDescription().RenderContextType)) return false;

		if (!m_ClientApp->OnInit()) return false;

		return true;
	}

	void Application::Tick() const
	{
		m_Window->PollEvents();

		if (m_Window->GetIsKeyDown(Swindow::KeyCode::Escape))
		{
			m_Window->SetIsRunning(false);
		}	

		m_ClientApp->OnTick();

		m_Window->SwapBuffers();
	}

	void Application::Destroy() const
	{
		m_ClientApp->OnDestroy();

		m_Window->Destroy();

		m_ClientApp->OnLateDestroy();
	}
}
