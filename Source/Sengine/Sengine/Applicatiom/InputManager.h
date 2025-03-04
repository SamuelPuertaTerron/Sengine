/**
 *	Filename: InputManager.h
 *	Date Created: 28/02/25
 *	Purpose: The main class that handles any input related code. 
 ***/

#pragma once
#include "KeyCodes.h"

namespace Sengine
{
	using AnyInputCode = std::any;

	using KeyDelegate = std::function<void()>;

	struct KeyStates
	{
		bool IsPressed;
		bool IsHeld;
		bool IsReleased;
	};

	template<typename T>
	struct TAction
	{
		KeyDelegate Callback;
		T Code;
	};

	struct KeyCodeAction :		TAction<EKeyCode> {};
	struct MouseButtonAction :	TAction<EMouseButton> {};

	struct ActionMap
	{
		std::string Name;
		std::unordered_map<std::string, TAction<AnyInputCode>> ActionList;

		bool operator==(const ActionMap& other) const;
	};

	class InputManager
	{
	public:
		static bool Init();
		static void Destroy();

		static void SaveEngineActionMap(const fs::path& path);
		static void LoadEngineActionMap(const fs::path& path);

		static bool IsKeyPressed(EKeyCode code);
		static bool IsKeyDown(EKeyCode code);

		static ActionMap CreateActionMap(const std::string& name);
		static void CreateActionMapping(ActionMap& map, const std::string& actionName, const AnyInputCode& code, const KeyDelegate& callback);

		static void Tick();

	private:
		static void KeyCallback(Swindow::KeyCode code, bool isPressed);
		static EKeyCode ConvertToSengineKeyCode(Swindow::KeyCode code);
	private:
		inline static std::unordered_map<EKeyCode, KeyStates> s_KeyStates;
		inline static std::vector<ActionMap> s_ActionMaps;
	};
}
