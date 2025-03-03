#include "Globals.h"
#include "InputManager.h"

namespace Sengine
{
	bool ActionMap::operator==(const ActionMap& other) const
	{
		return Name == other.Name;
	}

	bool InputManager::Init()
	{
		Application::GetInstance().GetWindow()->SetKeyCallback(KeyCallback);

		return true;
	}
	void InputManager::Destroy()
	{
	}
	bool InputManager::IsKeyPressed(KeyCode code)
	{
		return m_KeyStates[code].IsPressed;
	}

	bool InputManager::IsKeyDown(KeyCode code)
	{
		return !m_KeyStates[code].IsPressed;
	}

	void InputManager::KeyCallback(Swindow::KeyCode code, bool isPressed)
	{
		m_KeyStates[ConvertToSengineKeyCode(code)].IsPressed = isPressed;
	}

	KeyCode InputManager::ConvertToSengineKeyCode(Swindow::KeyCode code)
	{
		return static_cast<KeyCode>(code);
	}
}