/**
 *	Filename: Platform.h
 *	Date Created: 28/02/25
 *	Purpose: A static class to retrieve Platform specific functionality
 *	Last Modified: N/A
 *	Changes:	   N/A
 ***/

#pragma once

#include "IPlatformContext.h"

namespace Sengine
{
	enum class EMessageBoxType : uint8_t;

	class Platform
	{
	public:
		static bool Init();
		static void Destroy();

		static bool ShowMessageBox(const std::string& text, const std::string& title, EMessageBoxType type);
		static void OpenProcess(const std::string& command);

	private:
		inline static std::unique_ptr<IPlatformContext> m_PlatformContext;
	};
}//namespace Sengine