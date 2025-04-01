/**
 *	Filename: Win32NativeWindow.h
 *	Date Created: 29/03/25
 *	Purpose: The Win32 Input Manager. 
 ***/
#pragma once

namespace Sengine
{
	enum class EKeyCode : uint8_t;
}

namespace Sengine::Win32
{
	class Win32InputManager
	{
	public:
		static bool InputWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static EKeyCode ConvertNativeKeyCodes(int key);
	};

	class Win32KeyboardInput
	{
	public:
		static bool KeyboardWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	class Win32MouseInput
	{
	public:
		static bool MouseWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}//namespace Sengine::Win32