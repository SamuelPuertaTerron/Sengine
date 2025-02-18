#pragma once
#include <future>

#include "swindow/Swindow.h"

namespace Sengine
{
	struct WindowDescription : Swindow::WindowDescription {};

	class Window
	{
	public:
		Window() = default;
		~Window() = default;

		[[nodiscard]] std::shared_ptr<Window> Create(const WindowDescription& description);
		void Destroy() const;

		[[nodiscard]] bool GetIsRunning() const;
		void SetIsRunning(bool value) const;

		void PollEvents() const;
		void SwapBuffers() const;

		[[nodiscard]] bool GetIsKeyDown(Swindow::KeyCode code) const { return m_NativeWindow->GetIsKeyDown(code); }
	private:
		std::shared_ptr<Swindow::Window> m_NativeWindow;
	};
}

