/**
 *	Filename: Window.h
 *	Date Created: 21/02/25
 *	Purpose: The main window which abstracts away Swindow functionality
 *	Last Modified: 27/02/25
 *	Changes:	Added the GetIsKeyDown function to allow for easy and quick input
 *				Added Logging to both the create window and destroy window functions
 ***/

#pragma once
#include "Render/Context/RenderContextType.h"
#include "swindow/Swindow.h"

namespace Sengine
{
	enum class ERenderContextType : uint8_t;
}

namespace Sengine
{
	struct WindowDescription : Swindow::WindowDescription
	{
		ERenderContextType RenderContextType = ERenderContextType::OpenGL;
	};

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

