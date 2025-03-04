#include "Globals.h"
#include "Window.h"

namespace Sengine
{
	
	std::shared_ptr<Window> Window::Create(const WindowDescription& description)
	{
		m_NativeWindow = Swindow::Window::Create(description);

		m_NativeWindow->CreateContext(1, 0, true);

		Logger::Log("Created Window", ELogType::Info, ELogAreaType::Engine);
		return CreateSharedPtr<Window>();
	}

	void Window::Destroy() const
	{
		m_NativeWindow->Destroy();
		Logger::Log("Destroyed Window", ELogType::Info, ELogAreaType::Engine);
	}

	bool Window::GetIsRunning() const
	{
		return m_NativeWindow->GetIsRunning();
	}

	void Window::SetIsRunning(bool value) const
	{
		m_NativeWindow->SetIsRunning(value);
	}

	void Window::SetFullscreen() const
	{
		m_NativeWindow->SetFullscreen();
	}

	void Window::SetKeyCallback(const Swindow::WindowKeyCallback& callback) const
	{
		m_NativeWindow->SetWindowKeyCallback(callback);
	}

	void Window::PollEvents() const
	{
		m_NativeWindow->PollEvents();
	}

	void Window::SwapBuffers() const
	{
		m_NativeWindow->SwapBuffers();
	}

	SharedPtr<Swindow::Window> Window::GetNativeWindow() const
	{
		SE_ASSERT(!m_NativeWindow, "Native window has not been created.");

		return m_NativeWindow;
	}
	bool Window::GetIsKeyDown(Swindow::KeyCode code) const
	{
		return m_NativeWindow->GetIsKeyDown(code);
	}
	const char* GetProcAddress(const char* name)
	{
		return static_cast<const char*>(Application::GetInstance().GetWindow()->GetNativeWindow()->GetProcAddress(name));
	}
}
