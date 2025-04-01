/**
 *	Filename: Win32NativeWindow.h
 *	Date Created: 29/03/25
 *	Purpose: The Win32 window
 ***/
#pragma once

#include "Applicatiom/Window/INativeWindow.h"

namespace Sengine::Windows
{
	class Win32NativeWindow : public INativeWindow
	{
	public:
		Win32NativeWindow() = default;
		virtual ~Win32NativeWindow() override = default;

		virtual bool Init(const WindowDescription& description) override;
		virtual void Destroy() override;

		virtual void PollEvents() override;
		virtual void SwapBuffers() override;

		virtual void CreateGraphicsContext() override;

		virtual void* GetProcAddress(const char* name) override;
private:
		HWND m_WindowHandle;
		WINDOWPLACEMENT m_WindowPlacement;
		HDC m_DeviceContext;
		HINSTANCE m_Instance;
		HGLRC m_OpenGLContext;
	};
}//namespace Sengine::Windows