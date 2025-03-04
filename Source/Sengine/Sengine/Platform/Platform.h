/**
 *	Filename: Platform.h
 *	Date Created: 28/02/25
 *	Purpose: A static class to retrieve Platform specific functionality
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
		inline static UniquePtr<IPlatformContext> m_PlatformContext;
	};
}//namespace Sengine