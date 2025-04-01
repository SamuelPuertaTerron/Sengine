/**
 *	Filename: Win32NativeWindow.h
 *	Date Created: 29/03/25
 *	Purpose: The Win32 OpenGL context creation
 ***/
#pragma once

namespace Sengine::Windows
{
	class Win32OpenGLContext
	{
	public:
		static void CreateOpenGLContext(const HDC& dc, HGLRC& openglContext);
	};
}


