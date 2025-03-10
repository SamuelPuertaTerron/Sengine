/**
 *	Filename: Main.h
 *	Date Created: 27/02/25
 *	Purpose: The entry point for the Sengine engine.
 ***/

#pragma once
#include "Application.h"

std::shared_ptr<Sengine::ISengineApp> CreateApp();

namespace Sengine
{
	static int SengineMain()
	{
		std::shared_ptr<ISengineApp> app = CreateApp();
		SE_ASSERT(!app, "Could not create Application. Have you called CreateApp function inside the client app?");
		Application::GetInstance().RunApplication(app);
		return 0;
	}
}

//Entry Point

#ifdef SE_RELEASE
	#ifdef SE_PLATFORM_WINDOWS
		int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
		{
			return Sengine::SengineMain();
		}
	#endif	
#endif

#ifdef SE_DEBUG
int main()
{
	return Sengine::SengineMain();
}
#endif	