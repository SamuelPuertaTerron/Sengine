/**
 *	Filename: PlatformWindowsContext.h
 *	Date Created: 28/02/25
 *	Purpose: The class for the windows platform specific functionality
 ***/

#pragma once
#include "Core/Platform/IPlatformContext.h"

namespace Sengine::Win32
{
	class PlatformWindowsContext : public IPlatformContext
	{
	public:
		PlatformWindowsContext() = default;
		virtual ~PlatformWindowsContext() override = default;

		virtual bool ShowMessageBox(const std::string& text, const std::string& title, EMessageBoxType type) override;
		virtual void OpenProcess(const std::string& command) override;
	};
}//namespace Sengine::Windows


