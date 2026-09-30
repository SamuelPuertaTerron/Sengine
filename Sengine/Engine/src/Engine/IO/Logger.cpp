#include "Globals.h"
#include "Logger.h"

#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <format>
#include <string>
#include <string_view>

namespace Sengine::Logging
{
	static std::string_view LogToString(ELogType type)
	{
		switch (type)
		{
		case ELogType::Trace:   return "TRACE";
		case ELogType::Debug:   return "DEBUG";
		case ELogType::Info:	return "INFO";
		case ELogType::Warning: return "WARN";
		case ELogType::Error:   return "ERROR";
		case ELogType::Fatal:   return "FATAL";
		default:                return "LOG";
		}
	}

	static std::string_view LevelToString(int msgType)
	{
		switch (msgType)
		{
		case Raylib::LOG_TRACE:   return "TRACE";
		case Raylib::LOG_DEBUG:   return "DEBUG";
		case Raylib::LOG_INFO:    return "INFO";
		case Raylib::LOG_WARNING: return "WARN";
		case Raylib::LOG_ERROR:   return "ERROR";
		case Raylib::LOG_FATAL:   return "FATAL";
		default:                  return "LOG";
		}
	}

	static std::string FormatVaList(const char* text, va_list args)
	{
		va_list argsCopy;
		va_copy(argsCopy, args);
		const int size = std::vsnprintf(nullptr, 0, text, argsCopy);
		va_end(argsCopy);

		if (size <= 0)
		{
			return {};
		}

		std::string message(static_cast<size_t>(size), '\0');
		std::vsnprintf(message.data(), message.size() + 1, text, args);
		return message;
	}

	static std::string CurrentTimestamp()
	{
		const auto now = std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
		const std::chrono::zoned_time localTime{ std::chrono::current_zone(), now };
		return std::format("{:%Y-%m-%d %H:%M:%S}", localTime);
	}

	void CustomTraceLog(int msgType, const char* text, va_list args)
	{
		const std::string line = std::format("[{}] [{:<5}] {}\n",
			CurrentTimestamp(), LevelToString(msgType), FormatVaList(text, args));

		std::fputs(line.c_str(), stdout);
	}

	void Log(ELogType logType, const std::string& msg)
	{
		const std::string line = std::format("[{}] [{:<5}] {}\n",
			CurrentTimestamp(), LogToString(logType), msg);

		std::fputs(line.c_str(), stdout);
	}
}//namespace Sengine::Logging