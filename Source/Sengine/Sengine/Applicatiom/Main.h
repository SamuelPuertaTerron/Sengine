#pragma once
#include "Application.h"

Sengine::SengineAppPtr CreateApp();

namespace Sengine
{
	static int SengineMain()
	{
		Application sengineApp;
		auto app = CreateApp();
		SE_ASSERT(!app, "Could not create Application. Have you called CreateApp function inside the client app?");
		sengineApp.RunApplication(app);
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