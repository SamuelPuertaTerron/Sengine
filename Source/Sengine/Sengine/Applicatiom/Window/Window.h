/**
 *	Filename: Window.h
 *	Date Created: 21/02/25
 *	Purpose: The main window which abstracts away Swindow functionality
 ***/

#pragma once
#include "Render/Context/RenderContextType.h"
#include "Applicatiom/Window/INativeWindow.h"

namespace Sengine
{
	struct WindowDescription
	{
		std::string Title;
		Vector2Int Size;
		ERenderContextType RenderContextType = ERenderContextType::OpenGL;
	};

	using WindowResizeCallback = std::function<void(Vector2Int size)>;

	class Window
	{
	public:
		Window() = default;
		~Window() = default;

		[[nodiscard]] std::shared_ptr<Window> Create(const WindowDescription& description);
		void Destroy() const;

		void SetFullscreen() const;

		void PollEvents() const;
		void SwapBuffers() const;

		[[nodiscard]] std::shared_ptr<Windows::INativeWindow> GetNativeWindow() const;

	public:
		WindowResizeCallback WindowResizeCallback;
	private:
		void SetWindowSize(Vector2Int size);
		void SetWindowResizeCallback(Vector2Int size);
	private:
		std::shared_ptr<Windows::INativeWindow> m_NativeWindow;
		Vector2Int m_WindowSize;
	};

	const char* GetProcAddress(const char* name);
}

