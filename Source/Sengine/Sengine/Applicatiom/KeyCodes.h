/**
 *	Filename: KeyCodes.h
 *	Date Created: 28/02/25
 *	Purpose: An enum of all the key codes and mouse codes within the engine
 ***/

#pragma once

namespace Sengine
{
	//Enum representing key codes
	enum class EKeyCode : uint8_t
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
	enum class EMouseButton : uint8_t
	{
		Unknown = 0,
		LeftMouseButton,
		RightMouseButton,
	};
}