#include "Globals.h"

#include "Applicatiom/Platform/Win32/Win32NativeWindow.h"

namespace Sengine::Windows
{
	std::shared_ptr<INativeWindow> INativeWindow::CreateNativeWindow()
	{
#ifdef SE_PLATFORM_WINDOWS
		return std::make_shared<Win32NativeWindow>();
#endif
	}
}//namespace Sengine::Windows



