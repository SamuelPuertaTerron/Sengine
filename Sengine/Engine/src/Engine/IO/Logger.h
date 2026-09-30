#pragma once

namespace Sengine::Logging
{
	enum class ELogType
	{
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Fatal,
	};

	void CustomTraceLog(int msgType, const char* text, va_list args);

	void Log(ELogType msgType, const std::string& msg);

}//namespace Sengine::Logging
