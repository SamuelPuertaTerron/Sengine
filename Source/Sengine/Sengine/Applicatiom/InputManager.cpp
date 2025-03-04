#include "Globals.h"
#include "InputManager.h"

#include "InputManagerSerialization.h"

namespace Sengine
{
	inline static InputManagerSerialization m_InputSerialization = { {} };

	bool ActionMap::operator==(const ActionMap& other) const
	{
		return Name == other.Name;
	}

	bool InputManager::Init()
	{
		Application::GetInstance().GetWindow()->SetKeyCallback(KeyCallback);

		ActionMap engine = CreateActionMap("Engine Action Map");
		CreateActionMapping(engine, "Set Fullscreen", EKeyCode::F11, nullptr);
		CreateActionMapping(engine, "Quit", EKeyCode::Escape, nullptr);

		m_InputSerialization = InputManagerSerialization(engine);

		return true;
	}
	void InputManager::Destroy()
	{
	}

	void InputManager::SaveEngineActionMap(const fs::path& path)
	{
		Serializer::SerializeObject(path, m_InputSerialization);
	}

	void InputManager::LoadEngineActionMap(const fs::path& path)
	{

	}

	bool InputManager::IsKeyPressed(EKeyCode code)
	{
		return s_KeyStates[code].IsPressed;
	}

	bool InputManager::IsKeyDown(EKeyCode code)
	{
		return !s_KeyStates[code].IsHeld;
	}

	ActionMap InputManager::CreateActionMap(const std::string& name)
	{
		ActionMap map;
		map.Name = name;
		s_ActionMaps.push_back(map);
		return map;
	}

	void InputManager::CreateActionMapping(ActionMap& map, const std::string& actionName, const AnyInputCode& code, const KeyDelegate& callback)
	{
		TAction<AnyInputCode> action;
		action.Code = code;
		action.Callback = callback;
		map.ActionList[actionName] = action;
	}

	void InputManager::Tick()
	{
		for (const ActionMap& map : s_ActionMaps)
		{
			for (const auto& [actionName, action] : map.ActionList)
			{
				if (action.Code.type() == typeid(EKeyCode))
				{
					
				}
			}
		}
	}

	void InputManager::KeyCallback(Swindow::KeyCode code, bool isPressed)
	{
		if (isPressed)
			s_KeyStates[ConvertToSengineKeyCode(code)].IsPressed = true;
		else
			s_KeyStates[ConvertToSengineKeyCode(code)].IsPressed = false;
	}

	EKeyCode InputManager::ConvertToSengineKeyCode(Swindow::KeyCode code)
	{
		return static_cast<EKeyCode>(code);
	}
}
