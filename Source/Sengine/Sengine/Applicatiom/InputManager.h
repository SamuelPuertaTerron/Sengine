/**
 *	Filename: InputManager.h
 *	Date Created: 28/02/25
 *	Purpose: The main class that handles any input related code. 
 ***/

#pragma once
#include "KeyCodes.h"

namespace Sengine
{
	using KeyCallback = std::function<void()>;

	struct KeyStates
	{
		bool IsPressed;
		bool IsHeld;
		bool IsReleased;
	};

	template<typename T>
	struct TAction
	{
		KeyCallback Callback;
		T Code;
	};

	struct KeyCodeAction :		TAction<EKeyCode> {};
	struct MouseButtonAction :	TAction<EMouseButton> {};

	struct ActionMap
	{
		std::string Name;
		std::unordered_map<std::string, TAction<std::any>> ActionList;

		bool operator==(const ActionMap& other) const;
	};

	class InputManager
	{
	public:
		static bool Init();
		static void Destroy();

		static bool IsKeyPressed(KeyCode code);
		static bool IsKeyDown(KeyCode code);

	private:
		static void KeyCallback(Swindow::KeyCode code, bool isPressed);
		static KeyCode ConvertToSengineKeyCode(Swindow::KeyCode code);
	private:
		inline static std::unordered_map<KeyCode, KeyStates> m_KeyStates;
	};
}
