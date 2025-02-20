﻿// Swindow - Simple Window
// 
// Swindow is a lightweight, cross-platform library for managing windows and handling user input.
// Licensed under the MIT License. See LICENSE.txt for details.

#pragma once

#include <functional>
#include <future>
#include <iostream>
#include <string>
#include <typeindex>
#include <unordered_map>

#ifdef _WIN32
#include <Windows.h>
 //Include for OpenGL; including context creation.
#include <GL/GL.h>
#pragma comment (lib, "opengl32.lib")
#endif

#ifdef __LINUX__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#endif	

namespace Swindow
{
	//Forward Declarations

	enum class KeyCode : uint8_t;
	enum class MouseButton;

	class Window;

	namespace Internal
	{
		class NativeWindow;
		class RenderContext;
	}//Namespace Internal

	//Types

	using WindowPtr = std::shared_ptr<Window>;

	using Render = Internal::RenderContext;

	namespace Internal
	{
		using NativeWindowPtr = std::shared_ptr<Internal::NativeWindow>;
	}//Namespace Internal

	//Callbacks

	/**
	 * @brief Callback type for window close events.
	 *
	 * This callback is triggered when the user attempts to close the window.
	 *
	 * If a WindowCloseCallback is set, the application will invoke it.
	 * - If the callback returns `true`, the window will close, and the application will stop running.
	 * - If the callback returns `false`, the window will remain open, and the application will continue running.
	 *
	 * If no callback is set, the window will close by default.
	 *
	 * @return `true` to allow closing the window, `false` to keep it open.
	 */
	using WindowCloseCallback = std::function<bool()>;

	/**
	 * @brief Callback type for window resize events.
	 *
	 * This callback is triggered when the window size changes.
	 *
	 * @param width  The new width of the window.
	 * @param height The new height of the window.
	 */
	using WindowResizeCallback = std::function<void(int width, int height)>;

	/**
	 * @brief Callback type for handling key events.
	 *
	 * This callback is triggered when a key is pressed, released, or held.
	 *
	 * @param key    The key code of the pressed/released key.
	 * @param action The action type (press, release, repeat).
	 */
	using WindowKeyCallback = std::function<void(KeyCode key, bool action)>;

	/**
	 * @brief Callback type for mouse button events.
	 *
	 * This callback is triggered when a mouse button is pressed or released.
	 *
	 * @param button The mouse button being pressed/released.
	 * @param action The action type (press or release).
	 */
	using WindowMouseCallback = std::function<void(MouseButton button, bool action)>;

	/**
	 * @brief Callback type for mouse movement events.
	 *
	 * This callback is triggered when the mouse moves within the window.
	 *
	 * @param x The new x-coordinate of the mouse cursor.
	 * @param y The new y-coordinate of the mouse cursor.
	 */
	using WindowMouseMoveCallback = std::function<void(int x, int y)>;

	/**
	 * @brief Callback type for character input
	 *
	 * This callback is triggered when a character input event occurs,
	 * typically when a key is pressed and results in a printable character.
	 * It is useful for handling text input.
	 *
	 * @param character The result of the printable character, including special characters such as backspace ('\b'). 
	 */
	using WindowCharacterCallback = std::function<void(char character)>;

	//Class Declarations

	//Public

	//Window

	struct WindowDescription
	{
		std::string Title;
		int Width;
		int Height;
	};

	struct WindowCallbacks
	{
		//Basic Window Callbacks
		WindowResizeCallback WindowResizeCallback;
		WindowCloseCallback WindowCloseCallback;

		//Input Callbacks
		WindowKeyCallback WindowKeyCallback;
		WindowMouseCallback WindowMouseCallback;
		WindowMouseMoveCallback WindowMouseMoveCallback;
		WindowCharacterCallback WindowCharacterCallback;
	};

	class Window
	{
	public:
		/**
		 * @brief Default constructor. Should not be used directly.
		 *
		 * Use Window::Create instead to properly initialize a window.
		 */
		Window() = default;

		~Window() = default;

		/**
		 * @brief Creates the main application window.
		 *
		 * This function initializes and creates a new window with the specified settings.
		 *
		 * @param description The settings defining the properties of the window.
		 * @return A shared pointer to the newly created window.
		 */
		static WindowPtr Create(const WindowDescription& description);

		/**
		 * @brief Destroys the window.
		 *
		 * This function releases all resources associated with the window.
		 */
		void Destroy() const;

		/**
		 * @brief Checks if the window is still running.
		 *
		 * @return True if the window is running, false otherwise.
		 */
		bool GetIsRunning() const;

		/**
		 * @brief Sets the running state of the window.
		 *
		 * @param value True to keep the window running, false to stop it.
		 */
		void SetIsRunning(bool value);

		/**
		 * @brief Creates an OpenGL rendering context for the window.
		 *
		 * This function initializes an OpenGL context with the specified version.
		 * It supports both modern and legacy OpenGL versions.
		 *
		 * @param major The major OpenGL version (default is 4).
		 * @param minor The minor OpenGL version (default is 6).
		 * @param legacy Set to true to use legacy OpenGL (1.x or earlier) (default is false).
		 */
		void CreateContext(int major = 4, int minor = 6, bool legacy = false) const;

		/**
		 * @brief Swaps the front and back buffers.
		 *
		 * This function is typically called at the end of a render loop to present the next frame.
		 */
		void SwapBuffers() const;

		/**
		 * @brief Processes window events.
		 *
		 * This function handles user input and window events, such as resizing or closing.
		 * It should be called frequently within the main loop.
		 */
		void PollEvents() const;

		/**
		 * @brief Retrieves the address of an OpenGL function for the current context.
		 *
		 * @return A pointer to the requested function.
		 */
		char* GetProcAddress(const char* name) const;

		/**
		 * @brief Sets the callback function for window resizing.
		 *
		 * This callback is triggered when the window is resized.
		 *
		 * @param callback The function to be called when the window size changes.
		 */
		void SetWindowResizeCallback(WindowResizeCallback callback);

		/**
		* @brief Sets the callback function when the window is closing.
		*
		* This callback is triggered when the window is closing.
		*
		* @param callback The function to close the window.
		*/
		void SetWindowCloseCallback(WindowCloseCallback callback);

		/**
		 * @brief Sets the callback function for keyboard input.
		 *
		 * This callback is triggered when a key is pressed or released.
		 *
		 * @param callback The function to handle key events.
		 */
		void SetWindowKeyCallback(WindowKeyCallback callback);

		/**
		 * @brief Sets the callback function for mouse button events.
		 *
		 * This callback is triggered when a mouse button is pressed or released.
		 *
		 * @param callback The function to handle mouse button events.
		 */
		void SetWindowMouseCallback(WindowMouseCallback callback);

		/**
		 * @brief Sets the callback function for mouse movement.
		 *
		 * This callback is triggered when the mouse moves within the window.
		 *
		 * @param callback The function to handle mouse movement events.
		 */
		void SetWindowMouseMoveCallback(WindowMouseMoveCallback callback);

		void SetWindowCharacterCallback(WindowCharacterCallback callback);

		/**
		 * @brief Sets the window size.
		 *
		 * This function updates the dimensions of the window.
		 *
		 * @param width The new width of the window.
		 * @param height The new height of the window.
		 */
		void SetWindowSize(int width, int height);

		/**
		* @brief Is the key held down.
		*
		* This function checks if a key is held down.
		*
		* @param code The key code.
		*
		* @return True if the key is down, false if not
		*/
		bool GetIsKeyDown(KeyCode code);

		/**
		 * @brief Retrieves the window description.
		 *
		 * @return The current window description.
		 */
		WindowDescription GetWindowDescription() const { return m_WindowDescription; }

		/**
		 * @brief Retrieves the window callbacks.
		 *
		 * @return The set of callback functions associated with the window.
		 */
		WindowCallbacks GetWindowCallbacks() const { return m_WindowCallbacks; }

	private:
		WindowDescription m_WindowDescription;
		WindowCallbacks m_WindowCallbacks;
		Internal::NativeWindowPtr m_NativeWindow;
		bool m_IsRunning;
	};

	//Input

	//Enum representing key codes
	enum class KeyCode : uint8_t
	{
		Unknown = 0,

		A, B, C, D, E, F, G, H, I, J, K, L, M,
		N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

		Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

		Escape, Enter, Space, Backspace, Tab, Shift, Ctrl, Alt,
		Left, Right, Up, Down,

		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

		Count // Helper to get number of keys
	};

	//Enum representing mouse buttons
	enum class MouseButton
	{
		Unknown = 0,
		LeftMouseButton,
		RightMouseButton,
	};

	//Utility

	struct Colour
	{
		float R, G, B, A;

		Colour(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
			: R(r), G(g), B(b), A(a) {}
	};


	//Private
	namespace Internal
	{
		class Logger
		{
		public:
			static void Log(const std::string& message);
		};

		class Error : public std::runtime_error
		{
		public:
			Error(const std::string& message) : runtime_error(message) {}
		};

		class RenderContext
		{
		public:
			/**
			* @brief Sets the viewport size for rendering.
			*
			* This function updates the OpenGL viewport to match the given width and height.
			* It ensures that all rendering operations are scaled appropriately within the window.
			*
			* @param width The width of the viewport in pixels.
			* @param height The height of the viewport in pixels.
			*/
			static void SetViewportSize(int width, int height);

			/**
			* @brief Clears the current framebuffer.
			*
			* This function clears the color and depth buffers, preparing the screen for a new frame.
			* It should be called at the beginning of each render loop.
			*/
			static void Clear();

			/**
			 * @brief Draws a colored quad at a specified position and scale.
			 *
			 * This function renders a simple quad (square) at the given position.
			 * It can be used for basic UI elements, sprites, or debug rendering.
			 *
			 * @param x The X coordinate of the quad's position.
			 * @param y The Y coordinate of the quad's position.
			 * @param scale The scale factor of the quad (default is 1.0f).
			 * @param colour The color of the quad (default is an empty/neutral color).
			 */
			static void DrawQuad(float x, float y, float scale = 1.0f, Colour colour = {});
		};

		class NativeWindow
		{
		public:
			virtual ~NativeWindow() = default;

			static NativeWindowPtr Create(const WindowPtr& window);
			virtual void Destroy() {}

			virtual void RefreshScreen() {}
			virtual void PollEvents() {}

			virtual KeyCode ConvertNativeKeyCodes(int key) { return KeyCode::Unknown; }
			virtual int GetNativeKeyCodes(KeyCode code) { return -1; }

			virtual void CreateContext(int major = 4, int minor = 6, bool legacy = false) {}
			virtual void* GetExternalAddress(const char* name) { return nullptr; }

			virtual bool IsKeyDown(KeyCode code) { return false; }

			WindowPtr GetWindow() const { return m_Window; }

		protected:
			WindowPtr m_Window;
		};

#ifdef _WIN32

		class Win32NativeWindow : public NativeWindow
		{
		public:
			Win32NativeWindow(const WindowPtr& window);

			virtual ~Win32NativeWindow() override = default;

			virtual void Destroy() override;

			virtual void PollEvents() override;
			virtual void RefreshScreen() override;

			virtual KeyCode ConvertNativeKeyCodes(int key) override;
			virtual int GetNativeKeyCodes(KeyCode code) override;

			virtual void CreateContext(int major = 4, int minor = 6, bool legacy = false) override;

			virtual void* GetExternalAddress(const char* name) override;

			virtual bool IsKeyDown(KeyCode code) override;

		private:
			static void RegisterWindowClass(HINSTANCE hInstance);
		private:
			HWND m_WindowHandle;
			HDC m_DeviceContext;
			HINSTANCE m_Instance;
			HGLRC m_OpenGLContext;
		};

#endif

#ifdef __linux__
		class X11NativeWindow : public NativeWindow
		{
		public:
			X11NativeWindow(const WindowPtr& window);

			~X11NativeWindow() = default;

			virtual void Destroy() override;

			virtual void PollEvents() override;
			virtual void RefreshScreen() override;

			virtual KeyCode ConvertNativeKeyCodes(int key) override;

			virtual void CreateContext(int major = 4, int minor = 6, bool legacy = false) override;

			virtual void* GetExternalAddress(const char* name) override;
		private:
			Display* m_Display;
		};
#endif

	} //Namespace Internal


	//Class Implementations

	//Public

#pragma region Window

	inline WindowPtr Window::Create(const WindowDescription& description)
	{

		auto window = std::make_shared<Window>();
		window->m_WindowDescription = description;

		window->m_NativeWindow = Internal::NativeWindow::Create(window);

		window->m_IsRunning = true;

		Internal::Logger::Log("Created Window");

		return window; //Return the parent window
	}

	inline void Window::Destroy() const
	{
		m_NativeWindow->Destroy();
		Internal::Logger::Log("Destroyed Window");
	}

	inline bool Window::GetIsRunning() const
	{
		return m_IsRunning;
	}

	inline void Window::SetIsRunning(bool value)
	{
		m_IsRunning = value;
	}

	inline void Window::CreateContext(int major, int minor, bool legacy) const
	{
		m_NativeWindow->CreateContext(major, minor, legacy);
	}

	inline void Window::PollEvents() const
	{
		m_NativeWindow->PollEvents();
	}

	inline char* Window::GetProcAddress(const char* name) const
	{
		return static_cast<char*>(m_NativeWindow->GetExternalAddress(name));
	}

	inline void Window::SetWindowResizeCallback(WindowResizeCallback callback)
	{
		m_WindowCallbacks.WindowResizeCallback = std::move(callback);
	}

	inline void Window::SetWindowKeyCallback(WindowKeyCallback callback)
	{
		m_WindowCallbacks.WindowKeyCallback = std::move(callback);
	}

	inline void Window::SetWindowMouseCallback(WindowMouseCallback callback)
	{
		m_WindowCallbacks.WindowMouseCallback = std::move(callback);
	}

	inline void Window::SetWindowMouseMoveCallback(WindowMouseMoveCallback callback)
	{
		m_WindowCallbacks.WindowMouseMoveCallback = std::move(callback);
	}

	inline void Window::SetWindowCharacterCallback(WindowCharacterCallback callback)
	{
		m_WindowCallbacks.WindowCharacterCallback = std::move(callback);
	}

	inline void Window::SetWindowCloseCallback(WindowCloseCallback callback)
	{
		m_WindowCallbacks.WindowCloseCallback = callback;
	}

	inline void Window::SetWindowSize(int width, int height)
	{
		m_WindowDescription.Width = width;
		m_WindowDescription.Height = height;
	}

	inline bool Window::GetIsKeyDown(KeyCode code)
	{
		return m_NativeWindow->IsKeyDown(code);
	}

	inline void Window::SwapBuffers() const
	{
		m_NativeWindow->RefreshScreen();
	}
#pragma endregion

	//Private
	namespace Internal
	{
		inline void Logger::Log(const std::string& message)
		{
			std::cout << message << "\n";
		}

		inline NativeWindowPtr NativeWindow::Create(const WindowPtr& window)
		{
#ifdef _WIN32
			return std::make_shared<Win32NativeWindow>(window);
#endif
		}

#pragma region Render

		inline void RenderContext::SetViewportSize(int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		inline void RenderContext::Clear()
		{

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 1);
		}

		inline void RenderContext::DrawQuad(float x, float y, float scale, Colour colour)
		{
			// Enable blending for transparency
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Set up the translation and scaling transformations
			glPushMatrix();  // Save the current transformation matrix

			glTranslatef(x, y, 0.0f);  // Translate the quad to position (x, y)
			glScalef(scale, scale, 1.0f);  // Scale the quad by 'scale'

			// Set the color with an alpha value for transparency
			glColor4f(colour.R, colour.G, colour.B, colour.A);  // Red color with 25% opacity

			// Draw the quad
			glBegin(GL_QUADS);
			glVertex3f(-1.0f, -1.0f, 0.0f); // Bottom left
			glVertex3f(1.0f, -1.0f, 0.0f);  // Bottom right
			glVertex3f(1.0f, 1.0f, 0.0f);   // Top right
			glVertex3f(-1.0f, 1.0f, 0.0f);  // Top left
			glEnd();

			// Restore the previous transformation matrix
			glPopMatrix();

			// Disable blending if it's not needed for other objects
			glDisable(GL_BLEND);
		}


#pragma endregion


#ifdef _WIN32

		//OpenGL

		typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

		// These values come from the OpenGL and WGL extension specifications

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB   0x2093
#define WGL_CONTEXT_FLAGS_ARB         0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126

// Values for WGL_CONTEXT_FLAGS_ARB
#define WGL_CONTEXT_DEBUG_BIT_ARB              0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002

// Values for WGL_CONTEXT_PROFILE_MASK_ARB
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB		  0x0001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x0002


		inline Win32NativeWindow::Win32NativeWindow(const WindowPtr& window)
		{
			m_Window = window;

			m_Instance = GetModuleHandle(nullptr);

			RegisterWindowClass(m_Instance);

			//Defines the window size
			RECT rect = { 0, 0, m_Window->GetWindowDescription().Width, m_Window->GetWindowDescription().Height };
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

			//Create a temp title to avoid assertion
			const auto title = m_Window->GetWindowDescription().Title;
			const std::wstring wtitle(title.begin(), title.end());


			//Creates the main application window
			m_WindowHandle = CreateWindowEx(
				0, L"SwindowWindowClass",
				wtitle.c_str(),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
				nullptr, nullptr, m_Instance, m_Window.get()
			);

			if (!m_WindowHandle)
			{
				DWORD error = GetLastError();
				wchar_t errorMsg[256];
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error, 0, errorMsg, 256, nullptr);
				MessageBox(nullptr, errorMsg, L"CreateWindowEx Failed", MB_ICONERROR | MB_OK);

				return;
			}

			//Pass the window pointer to the WindowProc function
			SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

			m_DeviceContext = GetDC(m_WindowHandle);

			ShowWindow(m_WindowHandle, SW_SHOW);
			UpdateWindow(m_WindowHandle);
		}


		inline void Win32NativeWindow::Destroy()
		{
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(m_OpenGLContext);

			ReleaseDC(m_WindowHandle, m_DeviceContext);
			DestroyWindow(m_WindowHandle);

			UnregisterClass(L"SwindowWindowClass", GetModuleHandle(nullptr));

		}

		inline void Win32NativeWindow::PollEvents()
		{
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		inline void Win32NativeWindow::RefreshScreen()
		{
			SwapBuffers(m_DeviceContext);
		}

		inline KeyCode Win32NativeWindow::ConvertNativeKeyCodes(int key)
		{
			//Setup input map
			const std::unordered_map<int, KeyCode> keyMap = 
			{
				{ 'A', KeyCode::A }, { 'B', KeyCode::B }, { 'C', KeyCode::C }, { 'D', KeyCode::D },
				{ 'E', KeyCode::E }, { 'F', KeyCode::F }, { 'G', KeyCode::G }, { 'H', KeyCode::H },
				{ 'I', KeyCode::I }, { 'J', KeyCode::J }, { 'K', KeyCode::K }, { 'L', KeyCode::L },
				{ 'M', KeyCode::M }, { 'N', KeyCode::N }, { 'O', KeyCode::O }, { 'P', KeyCode::P },
				{ 'Q', KeyCode::Q }, { 'R', KeyCode::R }, { 'S', KeyCode::S }, { 'T', KeyCode::T },
				{ 'U', KeyCode::U }, { 'V', KeyCode::V }, { 'W', KeyCode::W }, { 'X', KeyCode::X },
				{ 'Y', KeyCode::Y }, { 'Z', KeyCode::Z },

				{ VK_ESCAPE, KeyCode::Escape }, { VK_RETURN, KeyCode::Enter },
				{ VK_SPACE, KeyCode::Space },   { VK_BACK, KeyCode::Backspace },
				{ VK_TAB, KeyCode::Tab },		   { VK_SHIFT, KeyCode::Shift },
				{ VK_CONTROL, KeyCode::Ctrl },	{ VK_MENU, KeyCode::Alt },

				{ VK_LEFT, KeyCode::Left },		{ VK_RIGHT, KeyCode::Right },
				{ VK_UP, KeyCode::Up },			{ VK_DOWN, KeyCode::Down },

				{ VK_F1, KeyCode::F1 },			{ VK_F2, KeyCode::F2 },		{ VK_F3, KeyCode::F3 },
				{ VK_F4, KeyCode::F4 },			{ VK_F5, KeyCode::F5 },		{ VK_F6, KeyCode::F6 },
				{ VK_F7, KeyCode::F7 },			{ VK_F8, KeyCode::F8 },		{ VK_F9, KeyCode::F9 },
				{ VK_F10, KeyCode::F10 },		{ VK_F11, KeyCode::F11 },	{ VK_F12, KeyCode::F12 }
			};

			auto it = keyMap.find(key);
			return (it != keyMap.end()) ? it->second : KeyCode::Unknown;
		}

		inline int Win32NativeWindow::GetNativeKeyCodes(KeyCode code)
		{
			//Setup input map
			const std::unordered_map<KeyCode, int> keyMap = 
			{
				{ KeyCode::A, 'A' }, { KeyCode::B, 'B' }, { KeyCode::C, 'C' }, { KeyCode::D, 'D' },
				{ KeyCode::E, 'E' }, { KeyCode::F, 'F' }, { KeyCode::G, 'G' }, { KeyCode::H, 'H' },
				{ KeyCode::I, 'I' }, { KeyCode::J, 'J' }, { KeyCode::K, 'K' }, { KeyCode::L, 'L' },
				{ KeyCode::M, 'M' }, { KeyCode::N, 'N' }, { KeyCode::O, 'O' }, { KeyCode::P, 'P' },
				{ KeyCode::Q, 'Q' }, { KeyCode::R, 'R' }, { KeyCode::S, 'S' }, { KeyCode::T, 'T' },
				{ KeyCode::U, 'U' }, { KeyCode::V, 'V' }, { KeyCode::W, 'W' }, { KeyCode::X, 'X' },
				{ KeyCode::Y, 'Y' }, { KeyCode::Z, 'Z' },

				{ KeyCode::Escape, VK_ESCAPE }, { KeyCode::Enter, VK_RETURN },
				{ KeyCode::Space, VK_SPACE },   { KeyCode::Backspace, VK_BACK },
				{ KeyCode::Tab, VK_TAB },       { KeyCode::Shift, VK_SHIFT },
				{ KeyCode::Ctrl, VK_CONTROL },  { KeyCode::Alt, VK_MENU },

				{ KeyCode::Left, VK_LEFT },     { KeyCode::Right, VK_RIGHT },
				{ KeyCode::Up, VK_UP },         { KeyCode::Down, VK_DOWN },

				{ KeyCode::F1, VK_F1 },   { KeyCode::F2, VK_F2 },   { KeyCode::F3, VK_F3 },
				{ KeyCode::F4, VK_F4 },   { KeyCode::F5, VK_F5 },   { KeyCode::F6, VK_F6 },
				{ KeyCode::F7, VK_F7 },   { KeyCode::F8, VK_F8 },   { KeyCode::F9, VK_F9 },
				{ KeyCode::F10, VK_F10 }, { KeyCode::F11, VK_F11 }, { KeyCode::F12, VK_F12 }
			};


			auto it = keyMap.find(code);
			return (it != keyMap.end()) ? it->second : -1;
		}

		inline void Win32NativeWindow::CreateContext(int major, int minor, bool legacy)
		{
			constexpr PIXELFORMATDESCRIPTOR formatDescriptor =
			{
				sizeof(PIXELFORMATDESCRIPTOR),													// Size of the descriptor
				1,																				// Version number
				PFD_DRAW_TO_WINDOW |															// Support window drawing
				PFD_SUPPORT_OPENGL |															// Support OpenGL
				PFD_DOUBLEBUFFER,																// Enable double buffering
				PFD_TYPE_RGBA,																	// RGBA color mode
				32,																				// 32-bit color depth
				0, 0, 0, 0, 0, 0,              // Ignore color bits
				0,																				// No alpha buffer
				0,																				// Ignore shift bit
				0,																				// No accumulation buffer
				0, 0, 0, 0,								// Ignore accumulation bits
				24,																				// 24-bit z-buffer
				8,																				// 8-bit stencil buffer
				0,																				// No auxiliary buffer
				PFD_MAIN_PLANE,																	// Main drawing layer
				0,																				// Reserved
		0, 0, 0															// Layer masks ignored
			};

			const int format = ChoosePixelFormat(m_DeviceContext, &formatDescriptor);
			SetPixelFormat(m_DeviceContext, format, &formatDescriptor);

			//Create a dummy context to load extensions
			HGLRC tempContext = wglCreateContext(m_DeviceContext);
			wglMakeCurrent(m_DeviceContext, tempContext);

			//Load wglCreateContextAttrribsARB
			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
				(PFNWGLCREATECONTEXTATTRIBSARBPROC)GetExternalAddress("wglCreateContextAttribsARB");

			if (!wglCreateContextAttribsARB)
			{
				DWORD error = GetLastError();
				wchar_t errorMsg[256];
				int result = swprintf(errorMsg, sizeof(errorMsg), L"Error Code: %lu", error);
				MessageBox(nullptr, errorMsg, L"Context Creation Failed", MB_ICONERROR);
				return;
			}

			HGLRC glContext;

			if (legacy)
			{
				//Creates OpenGL version 1.x or earlier
				glContext = wglCreateContext(m_DeviceContext);
			}
			else
			{
				//Creates Modern OpenGL version

				//Specify the OpenGL specifications
				const int attribs[] =
				{
					WGL_CONTEXT_MAJOR_VERSION_ARB, major,
					WGL_CONTEXT_MINOR_VERSION_ARB, minor,
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
					WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
					0,
				};

				//Create the modern OpenGL context
				glContext = wglCreateContextAttribsARB(m_DeviceContext, nullptr, attribs);
			}


			if (!glContext)
			{
				MessageBox(nullptr, L"There was an error creating the OpenGL context", L"Error Creating Context", MB_ICONERROR);
				wglMakeCurrent(nullptr, nullptr);
				wglDeleteContext(tempContext);
				return;
			}

			//Delete the dummy context
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(tempContext);

			wglMakeCurrent(m_DeviceContext, glContext);
			m_OpenGLContext = glContext;
		}

		inline void* Win32NativeWindow::GetExternalAddress(const char* name)
		{
			void* proc = (void*)wglGetProcAddress(name);
			if (!proc)
			{
				//If fails we attempt to load the opengl32.dll
				HMODULE module = LoadLibraryA("opengl32.dll");
				if (module)
				{
					proc = (void*)GetProcAddress(module, name);
				}
			}
			return proc;
		}

		inline bool Win32NativeWindow::IsKeyDown(KeyCode code)
		{
			int nativeCode = GetNativeKeyCodes(code);
			return GetAsyncKeyState(nativeCode);
		}

		inline LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			Win32NativeWindow* windowPtr = (Win32NativeWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			//Return the Default if the window we get from the constructor is null. 
			if (!windowPtr)
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

			//Windows message loop
			switch (uMsg)
			{
			case WM_CHAR:
				if (windowPtr->GetWindow()->GetWindowCallbacks().WindowCharacterCallback)
				{
					char character = static_cast<char>(wParam);

					if (character == '\b')
					{
						windowPtr->GetWindow()->GetWindowCallbacks().WindowCharacterCallback('\b');
					}
					else
					{
						windowPtr->GetWindow()->GetWindowCallbacks().WindowCharacterCallback(character);
					}
				}
				break;
			case WM_SIZE:
				//Window Resize Event
				if (windowPtr->GetWindow()->GetWindowCallbacks().WindowResizeCallback)
				{
					int width = LOWORD(lParam);
					int height = HIWORD(lParam);

					windowPtr->GetWindow()->SetWindowSize(width, height);

					windowPtr->GetWindow()->GetWindowCallbacks().WindowResizeCallback(width, height);
				}
				break;
			case WM_KEYDOWN:
			case WM_KEYUP:
				if (windowPtr->GetWindow()->GetWindowCallbacks().WindowKeyCallback)
				{
					KeyCode key = windowPtr->ConvertNativeKeyCodes(static_cast<int>(wParam));
					const bool isPressed = (uMsg == WM_KEYDOWN);

					windowPtr->GetWindow()->GetWindowCallbacks().WindowKeyCallback(key, isPressed);
				}
				break;
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
				if (windowPtr->GetWindow()->GetWindowCallbacks().WindowMouseCallback)
				{
					MouseButton button = static_cast<MouseButton>(VK_LBUTTON);
					const bool isPressed = (uMsg == WM_LBUTTONDOWN);
					windowPtr->GetWindow()->GetWindowCallbacks().WindowMouseCallback(button, isPressed);
				}
				break;
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
				if (windowPtr->GetWindow()->GetWindowCallbacks().WindowMouseCallback)
				{
					MouseButton button = static_cast<MouseButton>(VK_RBUTTON);
					const bool isPressed = (uMsg == WM_LBUTTONDOWN);
					windowPtr->GetWindow()->GetWindowCallbacks().WindowMouseCallback(button, isPressed);
				}
				break;
			case WM_MOUSEMOVE:
				if (windowPtr->GetWindow()->GetWindowCallbacks().WindowMouseMoveCallback)
				{
					const int mouseX = LOWORD(lParam);
					const int mouseY = HIWORD(lParam);

					windowPtr->GetWindow()->GetWindowCallbacks().WindowMouseMoveCallback(mouseX, mouseY);
				}
				break;
			case WM_CLOSE:
				//If the user has the WindowCloseCallback: Then the application will check if that is true before returning.
				if (windowPtr->GetWindow()->GetWindowCallbacks().WindowCloseCallback)
				{
					if (windowPtr->GetWindow()->GetWindowCallbacks().WindowCloseCallback())
					{
						windowPtr->GetWindow()->SetIsRunning(false);
					}
					//Continue running the loop if returns false
				}
				else
				{
					windowPtr->GetWindow()->SetIsRunning(false);
				}
				return 0;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

			default:
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		inline void Win32NativeWindow::RegisterWindowClass(HINSTANCE hInstance)
		{
			static bool registered = false;
			if (registered) return;

			WNDCLASSEX wc = {};
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WindowProc;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wc.lpszClassName = L"SwindowWindowClass";
			wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

			if (!RegisterClassEx(&wc))
			{
				MessageBox(nullptr, L"Failed to register window class", L"Error", MB_ICONERROR | MB_OK);
			}
			registered = true;
		}

#endif

#ifdef __linux__
		inline X11NativeWindow::X11NativeWindow(const WindowPtr& window)
		{

		}
#endif

	}//Namespace Internal

}//Namespace Swindow

//Only define this if you want ImGui for your window.
//Please note you will have to install ImGui separately. 
#ifdef SW_IMGUI_IMPLEMENTATION

#include "imgui/imgui.h"

namespace SwindowImGui
{
	static Swindow::WindowPtr g_Window;

	static int ConvertKeyCode(Swindow::KeyCode code)
	{
		//Return the code value as an int to be used within ImGui. 
		return static_cast<int>(code);
	}

	inline bool ImGui_ImplSwindow_Init(Swindow::WindowPtr window, bool install_callbacks)
	{
		if (!window)
			return false;

		//TODO: Move to a struct
		g_Window = window;

		ImGuiIO& io = ImGui::GetIO();

		// Navigation keys
		io.KeyMap[ImGuiKey_Tab] = ConvertKeyCode(Swindow::KeyCode::Tab);
		io.KeyMap[ImGuiKey_LeftArrow] = ConvertKeyCode(Swindow::KeyCode::Left);
		io.KeyMap[ImGuiKey_RightArrow] = ConvertKeyCode(Swindow::KeyCode::Right);
		io.KeyMap[ImGuiKey_UpArrow] = ConvertKeyCode(Swindow::KeyCode::Up);
		io.KeyMap[ImGuiKey_DownArrow] = ConvertKeyCode(Swindow::KeyCode::Down);
		io.KeyMap[ImGuiKey_PageUp] =	-1;
		io.KeyMap[ImGuiKey_PageDown] =	-1;
		io.KeyMap[ImGuiKey_Home] =		-1;
		io.KeyMap[ImGuiKey_End] =		-1;
		io.KeyMap[ImGuiKey_Insert] =	-1;
		io.KeyMap[ImGuiKey_Delete] =	-1;

		// Modifier keys
		io.KeyMap[ImGuiKey_Backspace] = ConvertKeyCode(Swindow::KeyCode::Backspace);
		io.KeyMap[ImGuiKey_Space] = ConvertKeyCode(Swindow::KeyCode::Space);
		io.KeyMap[ImGuiKey_Enter] = ConvertKeyCode(Swindow::KeyCode::Enter);
		io.KeyMap[ImGuiKey_Escape] = ConvertKeyCode(Swindow::KeyCode::Escape);

		// Alphabet keys
		for (char c = 'A'; c <= 'Z'; ++c) {
			io.KeyMap[ImGuiKey_A + (c - 'A')] = c;
		}

		// Number keys (top row)
		for (char c = '0'; c <= '9'; ++c) {
			io.KeyMap[ImGuiKey_0 + (c - '0')] = c;
		}

		// Function keys (F1-F12)
		io.KeyMap[ImGuiKey_F1] = ConvertKeyCode(Swindow::KeyCode::F1);
		io.KeyMap[ImGuiKey_F2] = ConvertKeyCode(Swindow::KeyCode::F2);
		io.KeyMap[ImGuiKey_F3] = ConvertKeyCode(Swindow::KeyCode::F3);
		io.KeyMap[ImGuiKey_F4] = ConvertKeyCode(Swindow::KeyCode::F4);
		io.KeyMap[ImGuiKey_F5] = ConvertKeyCode(Swindow::KeyCode::F5);
		io.KeyMap[ImGuiKey_F6] = ConvertKeyCode(Swindow::KeyCode::F6);
		io.KeyMap[ImGuiKey_F7] = ConvertKeyCode(Swindow::KeyCode::F7);
		io.KeyMap[ImGuiKey_F8] = ConvertKeyCode(Swindow::KeyCode::F8);
		io.KeyMap[ImGuiKey_F9] = ConvertKeyCode(Swindow::KeyCode::F9);
		io.KeyMap[ImGuiKey_F10] = ConvertKeyCode(Swindow::KeyCode::F10);
		io.KeyMap[ImGuiKey_F11] = ConvertKeyCode(Swindow::KeyCode::F11);
		io.KeyMap[ImGuiKey_F12] = ConvertKeyCode(Swindow::KeyCode::F12);

		// Numpad keys
		io.KeyMap[ImGuiKey_Keypad0] = ConvertKeyCode(Swindow::KeyCode::Num0);
		io.KeyMap[ImGuiKey_Keypad1] = ConvertKeyCode(Swindow::KeyCode::Num1);
		io.KeyMap[ImGuiKey_Keypad2] = ConvertKeyCode(Swindow::KeyCode::Num2);
		io.KeyMap[ImGuiKey_Keypad3] = ConvertKeyCode(Swindow::KeyCode::Num3);
		io.KeyMap[ImGuiKey_Keypad4] = ConvertKeyCode(Swindow::KeyCode::Num4);
		io.KeyMap[ImGuiKey_Keypad5] = ConvertKeyCode(Swindow::KeyCode::Num5);
		io.KeyMap[ImGuiKey_Keypad6] = ConvertKeyCode(Swindow::KeyCode::Num6);
		io.KeyMap[ImGuiKey_Keypad7] = ConvertKeyCode(Swindow::KeyCode::Num7);
		io.KeyMap[ImGuiKey_Keypad8] = ConvertKeyCode(Swindow::KeyCode::Num8);
		io.KeyMap[ImGuiKey_Keypad9] = ConvertKeyCode(Swindow::KeyCode::Num9);
		io.KeyMap[ImGuiKey_KeypadEnter] = -1;

		// Special characters
		io.KeyMap[ImGuiKey_Semicolon] = -1;
		io.KeyMap[ImGuiKey_Equal] = -1;
		io.KeyMap[ImGuiKey_Comma] = -1;
		io.KeyMap[ImGuiKey_Minus] = -1;
		io.KeyMap[ImGuiKey_Period] = -1;
		io.KeyMap[ImGuiKey_Slash] = -1;
		io.KeyMap[ImGuiKey_LeftBracket] = -1;
		io.KeyMap[ImGuiKey_Backslash] = -1;

		if (install_callbacks) 
		{
			g_Window->SetWindowMouseMoveCallback([&](int x, int y)
				{
					io.MousePos.x = static_cast<float>(x);
					io.MousePos.y = static_cast<float>(y);
				});

			g_Window->SetWindowMouseCallback([&](Swindow::MouseButton button, bool isPressed)
				{
					if (button == Swindow::MouseButton::LeftMouseButton)
					{
						io.MouseDown[0] = static_cast<int>(button) && isPressed;
					}

					if (button == Swindow::MouseButton::RightMouseButton)
					{
						io.MouseDown[1] = static_cast<int>(button) && isPressed;
					}
				});

			g_Window->SetWindowCharacterCallback([&](char character)
				{
					//Allow text field input
					io.AddInputCharacter(character);
				});

			g_Window->SetWindowKeyCallback([&](Swindow::KeyCode key, bool isPressed)
				{
					//Get the current ImGui key and set it is down
					int imguiKey = ConvertKeyCode(key);
					if (imguiKey >= 0 && imguiKey < IM_ARRAYSIZE(io.KeysDown)) 
					{
						io.KeysDown[imguiKey] = isPressed;

					}
				});
		}

		return true;
	}

	inline void ImGui_ImplSwindow_Shutdown()
	{
		g_Window = nullptr;
	}

	inline void ImGui_ImplSwindow_NewFrame()
	{
		// Update display size
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(g_Window->GetWindowDescription().Width), static_cast<float>(g_Window->GetWindowDescription().Height));

		// Update delta time
		io.DeltaTime = 0.016f;
	}
} // Namespace SwindowImGui

#endif // SW_IMGUI_IMPLEMENTATION