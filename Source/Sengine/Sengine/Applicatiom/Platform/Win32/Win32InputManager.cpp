#include "Globals.h"
#include "Win32InputManager.h"

#include "Applicatiom/Input/InputManager.h"

namespace Sengine::Win32
{
	bool Win32InputManager::InputWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (Win32KeyboardInput::KeyboardWindowProc(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}

		if (Win32MouseInput::MouseWindowProc(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}

		return false;
	}

	EKeyCode Win32InputManager::ConvertNativeKeyCodes(int key)
	{
		const std::unordered_map<int, EKeyCode> keyMap =
		{
			{ 'A', EKeyCode::A }, { 'B', EKeyCode::B }, { 'C', EKeyCode::C }, { 'D', EKeyCode::D },
			{ 'E', EKeyCode::E }, { 'F', EKeyCode::F }, { 'G', EKeyCode::G }, { 'H', EKeyCode::H },
			{ 'I', EKeyCode::I }, { 'J', EKeyCode::J }, { 'K', EKeyCode::K }, { 'L', EKeyCode::L },
			{ 'M', EKeyCode::M }, { 'N', EKeyCode::N }, { 'O', EKeyCode::O }, { 'P', EKeyCode::P },
			{ 'Q', EKeyCode::Q }, { 'R', EKeyCode::R }, { 'S', EKeyCode::S }, { 'T', EKeyCode::T },
			{ 'U', EKeyCode::U }, { 'V', EKeyCode::V }, { 'W', EKeyCode::W }, { 'X', EKeyCode::X },
			{ 'Y', EKeyCode::Y }, { 'Z', EKeyCode::Z },

			{ VK_ESCAPE, EKeyCode::Escape }, { VK_RETURN, EKeyCode::Enter },
			{ VK_SPACE, EKeyCode::Space },   { VK_BACK, EKeyCode::Backspace },
			{ VK_TAB, EKeyCode::Tab },		   { VK_SHIFT, EKeyCode::Shift },
			{ VK_CONTROL, EKeyCode::Ctrl },	{ VK_MENU, EKeyCode::Alt },

			{ VK_LEFT, EKeyCode::Left },		{ VK_RIGHT, EKeyCode::Right },
			{ VK_UP, EKeyCode::Up },			{ VK_DOWN, EKeyCode::Down },

			{ VK_F1, EKeyCode::F1 },			{ VK_F2, EKeyCode::F2 },		{ VK_F3, EKeyCode::F3 },
			{ VK_F4, EKeyCode::F4 },			{ VK_F5, EKeyCode::F5 },		{ VK_F6, EKeyCode::F6 },
			{ VK_F7, EKeyCode::F7 },			{ VK_F8, EKeyCode::F8 },		{ VK_F9, EKeyCode::F9 },
			{ VK_F10, EKeyCode::F10 },		{ VK_F11, EKeyCode::F11 },	{ VK_F12, EKeyCode::F12 }
		};

		auto it = keyMap.find(key);
		return (it != keyMap.end()) ? it->second : EKeyCode::Unknown;
	}

    bool Win32KeyboardInput::KeyboardWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        bool result = false;

        switch (uMsg)
        {
        case WM_CHAR:
            // Handle character input if needed
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (InputManager::KeyCallback)
            {
                EKeyCode key = Win32InputManager::ConvertNativeKeyCodes(static_cast<int>(wParam));
                EInputAction action = (lParam & (1 << 30)) ? EInputAction::Repeat : EInputAction::Press;
                InputManager::KeyCallback(key, action);
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (InputManager::KeyCallback)
            {
                EKeyCode key = Win32InputManager::ConvertNativeKeyCodes(static_cast<int>(wParam));
                InputManager::KeyCallback(key, EInputAction::Release);
            }
            break;
        }

        return result;
    }

    bool Win32MouseInput::MouseWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_LBUTTONDOWN:
            if (InputManager::MouseCallback)
            {
                InputManager::MouseCallback(EMouseButton::Left, EInputAction::Press);
            }
            break;
        case WM_LBUTTONUP:
            if (InputManager::MouseCallback)
            {
                InputManager::MouseCallback(EMouseButton::Left, EInputAction::Release);
            }
            break;
        case WM_RBUTTONDOWN:
            if (InputManager::MouseCallback)
            {
                InputManager::MouseCallback(EMouseButton::Right, EInputAction::Press);
            }
            break;
        case WM_RBUTTONUP:
            if (InputManager::MouseCallback)
            {
                InputManager::MouseCallback(EMouseButton::Right, EInputAction::Release);
            }
            break;
        case WM_MBUTTONDOWN:
            if (InputManager::MouseCallback)
            {
                InputManager::MouseCallback(EMouseButton::Middle, EInputAction::Press);
            }
            break;
        case WM_MBUTTONUP:
            if (InputManager::MouseCallback)
            {
                InputManager::MouseCallback(EMouseButton::Middle, EInputAction::Release);
            }
            break;
        case WM_MOUSEMOVE:
            if (InputManager::MousePositionCallback)
            {
                const int mouseX = LOWORD(lParam);
                const int mouseY = HIWORD(lParam);
                InputManager::MousePositionCallback(mouseX, mouseY);
            }
            break;
        case WM_MOUSEWHEEL:
            if (InputManager::MouseScrollCallback)
            {
                float delta = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
                InputManager::MouseScrollCallback(0.0f, delta);
            }
            break;
        case WM_MOUSEHWHEEL:
            if (InputManager::MouseScrollCallback)
            {
                float delta = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
                InputManager::MouseScrollCallback(delta, 0.0f);
            }
            break;
        }

        return false;
    }
}//namespace Sengine::Win32