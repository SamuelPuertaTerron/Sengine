/**
 *	Filename: ApplicationState.h
 *	Date Created: 29/03/25
 *	Purpose: The current state of the application, includes runtime state
 ***/

#pragma once

namespace Sengine
{
	enum class ECurrentState
	{
		None = 0,
		Running,
		Paused,
		StartExit,
		Exited,
	};

	struct ApplicationState
	{
		ECurrentState CurrentState;

		[[nodiscard]] bool GetIsRunning() const;
		void SetIsRunning(bool value);
	};
}//namespace Sengine