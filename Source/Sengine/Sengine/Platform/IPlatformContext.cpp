#include "Globals.h"
#include "IPlatformContext.h"

#include "Windows/PlatformWindowsContext.h"

namespace Sengine
{
	UniquePtr<IPlatformContext> IPlatformContext::Create()
	{
#ifdef SE_PLATFORM_WINDOWS
		return CreateUniquePtr<Windows::PlatformWindowsContext>();
#endif//Se_PLATFORM_WINDOWS
	}
}//namespace Sengine