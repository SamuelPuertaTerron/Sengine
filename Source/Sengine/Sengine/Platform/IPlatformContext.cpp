#include "Globals.h"
#include "IPlatformContext.h"

#include "Windows/PlatformWindowsContext.h"

namespace Sengine
{
	std::unique_ptr<IPlatformContext> IPlatformContext::Create()
	{
#ifdef SE_PLATFORM_WINDOWS
		return std::make_unique<Windows::PlatformWindowsContext>();
#endif//Se_PLATFORM_WINDOWS
	}
}//namespace Sengine