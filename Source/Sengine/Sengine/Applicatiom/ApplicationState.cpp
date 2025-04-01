#include "Globals.h"
#include "ApplicationState.h"

namespace Sengine
{
	bool ApplicationState::GetIsRunning() const
	{
		return CurrentState == ECurrentState::Running;
	}

	void ApplicationState::SetIsRunning(bool value)
	{
		if (value)
		{
			CurrentState = ECurrentState::Running;
		}
		else
		{
			CurrentState = ECurrentState::StartExit;
		}
	}
}//namespace Sengine