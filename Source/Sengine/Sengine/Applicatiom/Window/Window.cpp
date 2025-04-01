#include "Globals.h"
#include "Window.h"

#include "Render/Renderer.h"

namespace Sengine
{
	
	std::shared_ptr<Window> Window::Create(const WindowDescription& description)
	{
		m_NativeWindow = Windows::INativeWindow::CreateNativeWindow();
		if (!m_NativeWindow->Init(description))
		{
			return nullptr;
		}

		Logger::Log("Created Window", ELogType::Info, ELogAreaType::Engine);


		SetWindowSize(description.Size);

		WindowResizeCallback = [&](Vector2Int size)
			{
				SetWindowResizeCallback(size);
			};

		return std::make_shared<Window>();
	}

	void Window::Destroy() const
	{
		m_NativeWindow->Destroy();
		Logger::Log("Destroyed Window", ELogType::Info, ELogAreaType::Engine);
	}

	void Window::SetFullscreen() const
	{
		//m_NativeWindow->SetFullscreen();
	}

	void Window::PollEvents() const
	{
		m_NativeWindow->PollEvents();
	}

	void Window::SwapBuffers() const
	{
		m_NativeWindow->SwapBuffers();
	}

	std::shared_ptr<Windows::INativeWindow> Window::GetNativeWindow() const
	{
		SE_ASSERT(!m_NativeWindow, "Native window has not been created.");

		return m_NativeWindow;
	}

	void Window::SetWindowSize(Vector2Int size)
	{
		m_WindowSize = size;
		Renderer::SetViewportSize(size);

		Logger::Log("Set Window Size to: " + m_WindowSize.ToString(), ELogType::Info, ELogAreaType::Engine);
	}

	void Window::SetWindowResizeCallback(Vector2Int size)
	{
		SetWindowSize(size);
	}

	const char* GetProcAddress(const char* name)
	{
		return static_cast<const char*>(Application::GetInstance().GetWindow()->GetNativeWindow()->GetProcAddress(name));
	}
}
