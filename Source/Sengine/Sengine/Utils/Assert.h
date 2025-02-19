#pragma once

#include <fstream>
#include <cstdlib>

namespace Sengine
{
	class Assertion
	{
	public:
		static void Assert(bool condition, const char* conditionStr, const char* file, int line, const char* message = "")
		{
			if (condition)
			{
				// Log assertion failure to file
				std::ofstream logFile("AssertLog.txt", std::ios::app);
				if (logFile.is_open())
				{
					logFile << "Assertion failed in file " << file << " at line: " << line << "\n"
						<< "Condition: " << conditionStr << "\n"
						<< "Message: " << message << "\n\n";
					logFile.close();
				}

				std::abort();
			}
		}
	};
} // namespace Sengine

#define SE_Assert(cond, msg) Sengine::Assertion::Assert((cond), #cond, __FILE__, __LINE__, msg)
