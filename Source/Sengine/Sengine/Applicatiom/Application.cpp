#include "Globals.h"
#include "Application.h"

#include "InputManager.h"
#include "Window.h"

namespace Sengine
{
	Application& Application::GetInstance()
	{
		if (!s_Instance)
		{
			s_Instance = new Application();
		}

		return *s_Instance;
	}

	void Application::RunApplication(const std::shared_ptr<ISengineApp>& app)
	{
		m_ClientApp = app;

		if (!Init())
		{
			Destroy();
			return;
		}

		while (m_Window->GetIsRunning())
		{
			Tick();
		}

		Destroy();
	}

	std::shared_ptr<ISengineApp> Application::GetClientApp() const
	{
		SE_ASSERT(!m_ClientApp, "Client App is null. Don't call the GetClientApp function until the client app has been created");

		return m_ClientApp;
	}

	std::shared_ptr<Window> Application::GetWindow() const
	{
		SE_ASSERT(!m_Window, "Window is null. Don't call the GetWindow function until the window has been created");

		return m_Window;
	}

	bool Application::Init()
	{
		if (!m_ClientApp->OnEarlyInit()) return false;

		m_Window = std::make_shared<Window>();

		if (!m_Window->Create(m_ClientApp->GetWindowDescription())) return false;

		if (!Renderer::Init(m_ClientApp->GetWindowDescription().RenderContextType)) return false;

		if (!InputManager::Init()) return false;

		if (!Platform::Init()) return false;

		if (!m_ClientApp->OnInit()) return false;

		return true;
	}

	void Application::Tick() const
	{
		m_Window->PollEvents();

		m_ClientApp->OnTick();

		m_Window->SwapBuffers();
	}

	void Application::Destroy() const
	{
		if (m_ClientApp)
			m_ClientApp->OnDestroy();

		Platform::Destroy();

		Renderer::Destroy();

		if (m_Window)
			m_Window->Destroy();

		if (m_ClientApp)
			m_ClientApp->OnLateDestroy();

		//The last thing that should be deleted
		delete s_Instance;
	}
}
