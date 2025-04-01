#include "Globals.h"
#include "Win32OpenGLContext.h"

//OpenGL

#include <GL/GL.h>
#pragma comment (lib, "opengl32.lib")

namespace Sengine::Windows
{
	namespace OpenGL
	{
		typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

		// These values come from the OpenGL and WGL extension specifications

		constexpr auto WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091;
		constexpr auto WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092;
		constexpr auto WGL_CONTEXT_LAYER_PLANE_ARB = 0x2093;
		constexpr auto WGL_CONTEXT_FLAGS_ARB = 0x2094;
		constexpr auto WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126;

		// Values for WGL_CONTEXT_FLAGS_ARB
		constexpr auto WGL_CONTEXT_DEBUG_BIT_ARB = 0x0001;
		constexpr auto WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = 0x0002;

		// Values for WGL_CONTEXT_PROFILE_MASK_ARB
		constexpr auto WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x0001;
		constexpr auto WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = 0x0002;
	}//namespace OpenGL

	void Win32OpenGLContext::CreateOpenGLContext(const HDC& dc, HGLRC& openglContext)
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

		const int format = ChoosePixelFormat(dc, &formatDescriptor);
		SetPixelFormat(dc, format, &formatDescriptor);

		//Create a dummy context to load extensions
		HGLRC tempContext = wglCreateContext(dc);
		wglMakeCurrent(dc, tempContext);

		//Load wglCreateContextAttrribsARB
		OpenGL::PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
			(OpenGL::PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

		if (!wglCreateContextAttribsARB)
		{
			DWORD error = GetLastError();
			wchar_t errorMsg[256];
			int result = swprintf(errorMsg, sizeof(errorMsg), L"Error Code: %lu", error);
			MessageBox(nullptr, errorMsg, L"Context Creation Failed", MB_ICONERROR);
			return;
		}

		//Specify the OpenGL specifications
		const int attribs[] =
		{
			OpenGL::WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			OpenGL::WGL_CONTEXT_MINOR_VERSION_ARB, 6,
			OpenGL::WGL_CONTEXT_PROFILE_MASK_ARB, OpenGL::WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			OpenGL::WGL_CONTEXT_FLAGS_ARB, OpenGL::WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0,
		};

		//Create the modern OpenGL context
		HGLRC glContext = wglCreateContextAttribsARB(dc, nullptr, attribs);

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

		wglMakeCurrent(dc, glContext);
		openglContext = glContext;
	}
}//namespace Sengine::Windows