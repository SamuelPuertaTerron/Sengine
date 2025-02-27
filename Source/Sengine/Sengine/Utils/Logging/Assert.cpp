#include "Globals.h"
#include "Assert.h"

namespace Sengine
{
	void Assertion::Assert(bool condition, const char* conditionStr, const char* file, int line, const char* message)
	{
		if (condition)
		{
			std::stringstream ss;
			ss << condition << " at file " << file << " at line " <<  line << "\n" << "Message: " << message << "\n";
			Logger::Log(ss.str(), ELogType::Fatal, ELogAreaType::Assert);
			std::abort();
		}
	}
}