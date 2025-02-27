#pragma once

#include <fstream>
#include <cstdlib>

namespace Sengine
{
	class Logger;
}

namespace Sengine
{
	class Assertion
	{
	public:
		static void Assert(bool condition, const char* conditionStr, const char* file, int line, const char* message = "");
	};
} // namespace Sengine

#define SE_ASSERT(cond, msg) Sengine::Assertion::Assert((cond), #cond, __FILE__, __LINE__, msg)
