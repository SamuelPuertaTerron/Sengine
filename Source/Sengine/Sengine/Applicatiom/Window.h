/**
 *	Filename: Window.h
 *	Date Created: 21/02/25
 *	Purpose: The main window which abstracts away Swindow functionality
 ***/

#pragma once
#include "Render/Context/RenderContextType.h"
#define SE_OPENGL_IMPLEMENTATION
#ifdef SE_OPENGL_IMPLEMENTATION
#define SW_OPENGL_IMPLEMENTATION
#include "glad/include/glad/glad.h"
#include "swindow/Swindow.h"
#endif

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

		void SetFullscreen() const;

		void SetKeyCallback(const Swindow::WindowKeyCallback& callback) const;

		void PollEvents() const;
		void SwapBuffers() const;

		[[nodiscard]] std::shared_ptr<Swindow::Window> GetNativeWindow() const;

		[[nodiscard]] bool GetIsKeyDown(Swindow::KeyCode code) const;
	private:
		std::shared_ptr<Swindow::Window> m_NativeWindow;
	};

	const char* GetProcAddress(const char* name);

	void IsKeyPressed(Swindow::KeyCode code, bool isPressed);
}

