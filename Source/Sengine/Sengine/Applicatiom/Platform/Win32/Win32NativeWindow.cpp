#include "Globals.h"
#include "Win32NativeWindow.h"

#include "Win32InputManager.h"
#include "Win32OpenGLContext.h"

namespace Sengine::Windows
{
	constexpr LPCWSTR CLASS_NAME = L"SengineWin32WindowClass";

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	bool Win32NativeWindow::Init(const WindowDescription& description) {
		m_Instance = GetModuleHandle(nullptr);

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = m_Instance;
		wc.lpszClassName = CLASS_NAME;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

		if (!RegisterClassEx(&wc)) {
			MessageBox(nullptr, L"RegisterClassEx failed", L"Error", MB_ICONERROR);
			return false; // ← Exit on failure
		}

		RECT rect = { 0, 0, (LONG)description.Size.X, (LONG)description.Size.Y };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		std::wstring wtitle(description.Title.begin(), description.Title.end());
		m_WindowHandle = CreateWindowEx(
			0,
			CLASS_NAME,
			wtitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr, nullptr, m_Instance, nullptr
		);

		if (!m_WindowHandle) {
			MessageBox(nullptr, L"CreateWindowEx returned NULL", L"Error", MB_ICONERROR);
			return false;
		}

		m_DeviceContext = GetDC(m_WindowHandle);
		ShowWindow(m_WindowHandle, SW_SHOW);
		UpdateWindow(m_WindowHandle);
		return true;
	}

	void Win32NativeWindow::Destroy()
	{
		ReleaseDC(m_WindowHandle, m_DeviceContext);
		DestroyWindow(m_WindowHandle);

		UnregisterClass(CLASS_NAME, GetModuleHandle(nullptr));
	}

	void Win32NativeWindow::PollEvents()
	{
		MSG msg;
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			WaitMessage();
		}
	}

	void Win32NativeWindow::SwapBuffers()
	{
		::SwapBuffers(m_DeviceContext);
	}

	void Win32NativeWindow::CreateGraphicsContext()
	{
		switch (Application::GetInstance().GetWindowDescription().RenderContextType)
		{
		case ERenderContextType::OpenGL:
			Win32OpenGLContext::CreateOpenGLContext(m_DeviceContext, m_OpenGLContext);
			break;
		case ERenderContextType::DX11:
		case ERenderContextType::Vulkan:
		case ERenderContextType::None:
			SE_ASSERT(Application::GetInstance().GetWindowDescription().RenderContextType != ERenderContextType::OpenGL, 
				"The current Graphics API supported is OpenGL");
			break;
		}
	}

	void* Win32NativeWindow::GetProcAddress(const char* name)
	{
		void* proc = (void*)wglGetProcAddress(name);
		if (!proc)
		{
			//If fails we attempt to load the opengl32.dll
			HMODULE module = LoadLibraryA("opengl32.dll");
			if (module)
			{
				proc = (void*)::GetProcAddress(module, name);
			}
		}
		return proc;
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			if (Application::GetInstance().GetWindow()->WindowResizeCallback)
			{
				Vector2Int size = { LOWORD(lParam), HIWORD(lParam) };
				Application::GetInstance().GetWindow()->WindowResizeCallback(size);
			}
			break;
		case WM_DESTROY:
			Application::GetInstance().Quit();
			break;
		case WM_QUIT:
			break;
		}

		if (Win32::Win32InputManager::InputWindowProc(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}//namespace Sengine::Windows
