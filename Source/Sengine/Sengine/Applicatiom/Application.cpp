#include "Application.h"

#include "Window.h"

namespace Sengine
{
	void Application::CreateApplication(const std::shared_ptr<ISengineApp>& app)
	{
		m_ClientApp = app;

		if (!Init()) return;

		Tick();

		Destroy();
	}

	bool Application::Init()
	{
		if (!m_ClientApp->OnEarlyInit()) return false;

		m_Window = std::make_shared<Window>();

		if (!m_Window->Create(m_ClientApp->GetWindowDescription())) return false;

		if (!m_ClientApp->OnInit()) return false;

		return true;
	}

	void Application::Tick()
	{
		while (m_Window->GetIsRunning())
		{
			m_Window->PollEvents();

			if (m_Window->GetIsKeyDown(Swindow::KeyCode::Escape))
			{
				m_Window->SetIsRunning(false);
			}	

			m_ClientApp->OnTick();

			m_Window->SwapBuffers();
		}
	}

	void Application::Destroy()
	{
		m_ClientApp->OnDestroy();

		m_Window->Destroy();

		m_ClientApp->OnLateDestroy();
	}
}
