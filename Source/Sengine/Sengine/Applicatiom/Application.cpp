#include "Globals.h"
#include "Application.h"

#include "Applicatiom/Input/InputManager.h"
#include "Applicatiom/Window/Window.h"
#include "Render/Renderer.h"

namespace Sengine
{
	Application& Application::GetInstance()
	{
		if (!s_pInstance)
		{
			s_pInstance = new Application();
		}

		return *s_pInstance;
	}

	void Application::RunApplication(const std::shared_ptr<ISengineApp>& app)
	{
		m_ClientApp = app;

		if (!Init())
		{
			Destroy();
			SE_ASSERT(true, "");
			return;
		}

		while (m_pAppState->GetIsRunning())
		{
			Tick();
		}

		Destroy();
	}

	void Application::Quit() const
	{
		m_pAppState->SetIsRunning(false);
	}

	ApplicationState* Application::GetState() const
	{
		return m_pAppState;
	}

	WindowDescription Application::GetWindowDescription() const
	{
		return m_ClientApp->GetWindowDescription();
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
		if (!EarlyInit())
		{
			return false;
		}

		if (!ApplicationInit())
		{
			return false;
		}

		if (!LateInit())
		{
			return false;
		}

		return true;
	}

	void Application::Tick() const
	{
		m_Window->PollEvents();

		InputManager::Tick(0.016f);

		m_ClientApp->OnTick();

		m_Window->SwapBuffers();
	}

	void Application::Destroy() const
	{
		if (m_ClientApp)
		{
			m_ClientApp->OnDestroy();
		}

		InputManager::Destroy();

		Renderer::Destroy();

		if (m_Window)
		{
			m_Window->Destroy();
		}

		Platform::Destroy();

		if (m_ClientApp)
		{
			m_ClientApp->OnLateDestroy();
		}

		//The last thing that should be deleted
		delete m_pAppState;
		delete s_pInstance;
	}

	bool Application::EarlyInit()
	{
		m_pAppState = new ApplicationState();
		m_pAppState->SetIsRunning(true);

		//TODO: Future Init Core types here, including memory.

		if (!m_ClientApp->OnEarlyInit())
		{
			return false;
		}

		if (!Platform::Init())
		{
			return false;
		}

		return true;
	}

	bool Application::ApplicationInit()
	{
		m_Window = std::make_shared<Window>();
		if (!m_Window->Create(m_ClientApp->GetWindowDescription()))
		{
			return false;
		}

		if (!Renderer::Init(m_ClientApp->GetWindowDescription().RenderContextType))
		{
			return false;
		}

		if (!InputManager::Init())
		{
			return false;
		}

		return true;
	}

	bool Application::LateInit()
	{
		if (!m_ClientApp->OnInit())
		{
			return false;
		}

		return true;
	}
}//namespace Sengine