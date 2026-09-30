#include "Globals.h"
#include "Input.h"

namespace Sengine::Input
{
	bool IsKeyPressed(EKeyCode keyCode)
	{
		return Raylib::IsKeyPressed(static_cast<int>(keyCode));
	}

	bool IsKeyDown(EKeyCode keyCode)
	{
		return Raylib::IsKeyDown(static_cast<int>(keyCode));
	}

	bool IsMouseButtonPressed(EMouseButton button)
	{
		return Raylib::IsMouseButtonPressed(static_cast<int>(button));
	}

	bool IsMouseButtonDown(EMouseButton button)
	{
		return Raylib::IsMouseButtonDown(static_cast<int>(button));
	}
	void ShouldShowMouseCursor(bool value)
	{
		if (value)
		{
			Raylib::ShowCursor();
		}
		else
		{
			Raylib::HideCursor();
		}
	}
	void ShouldLockMouseCursor(bool value)
	{
		if (value)
		{
			Raylib::DisableCursor();
		}
		else
		{
			Raylib::EnableCursor();
		}
	}
}//namespace Sengine
