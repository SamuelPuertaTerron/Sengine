#include "Globals.h"
#include "Platform.h"

#include "IPlatformContext.h"

namespace Sengine
{
	bool Platform::Init()
	{
		m_PlatformContext = IPlatformContext::Create();

		return true;
	}

	void Platform::Destroy()
	{

	}

	bool Platform::ShowMessageBox(const std::string& text, const std::string& title, EMessageBoxType type)
	{
		return m_PlatformContext->ShowMessageBox(text, title, type);
	}

	void Platform::OpenProcess(const std::string& command)
	{

	}
}//namespace Sengine
