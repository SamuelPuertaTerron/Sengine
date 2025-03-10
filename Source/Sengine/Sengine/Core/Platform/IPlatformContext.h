/**
 *	Filename: IPlatformContext.h
 *	Date Created: 27/02/25
 *	Purpose: Abstracts Platform specific functions like dialogs and message boxes
 ***/

#pragma once

namespace Sengine
{
	enum class EMessageBoxType : uint8_t
	{
		None = 0,
		Info,
		Warning,
		Error,
	};

	class IPlatformContext
	{
	public:
		virtual ~IPlatformContext() = default;

		virtual bool ShowMessageBox(const std::string& text, const std::string& title, EMessageBoxType type) = 0;
		virtual void OpenProcess(const std::string& command) = 0;

		static std::unique_ptr<IPlatformContext> Create();

	};
}

