#include "Window.h"

namespace Sengine
{
	std::shared_ptr<Window> Window::Create(const WindowDescription& description)
	{
		m_NativeWindow = Swindow::Window::Create(description);

		m_NativeWindow->CreateContext();

		return std::make_shared<Window>();
	}

	void Window::Destroy() const
	{
		m_NativeWindow->Destroy();
	}

	bool Window::GetIsRunning() const
	{
		return m_NativeWindow->GetIsRunning();
	}

	void Window::SetIsRunning(bool value) const
	{
		m_NativeWindow->SetIsRunning(value);
	}

	void Window::PollEvents() const
	{
		m_NativeWindow->PollEvents();
	}

	void Window::SwapBuffers() const
	{
		m_NativeWindow->SwapBuffers();
	}
}
