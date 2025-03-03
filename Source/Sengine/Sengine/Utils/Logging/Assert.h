/**
 *	Filename: Assert.h
 *	Date Created: 21/02/25
 *	Purpose: Functionality to Assert and stop the program from currently running
 ***/
#pragma once

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
