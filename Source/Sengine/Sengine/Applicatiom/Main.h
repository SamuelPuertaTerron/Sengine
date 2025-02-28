/**
 *	Filename: Main.h
 *	Date Created: 27/02/25
 *	Purpose: The entry point for the Sengine engine.
 *	Last Modified: 28/02/25
 *	Changes:	   Modified the SengineMain function to use the singleton Application class
 ***/

#pragma once
#include "Application.h"

Sengine::SengineAppPtr CreateApp();

namespace Sengine
{
	static int SengineMain()
	{
		auto app = CreateApp();
		SE_ASSERT(!app, "Could not create Application. Have you called CreateApp function inside the client app?");
		Application::GetInstance().RunApplication(app);
		return 0;
	}
}

//Entry Point

#ifdef SE_RELEASE
	#ifdef SE_PLATFORM_WINDOWS
		inline int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
		{
			return Sengine::SengineMain();
		}
	#endif	
#endif

#ifdef SE_DEBUG
inline int main()
{
	return Sengine::SengineMain();
}
#endif	