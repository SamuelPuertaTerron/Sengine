#pragma once
#include <string>
#include <filesystem>

namespace Sengine::Platform
{
	struct ProcessResult
	{
		bool Started = false;
		int ExitCode = -1;
		std::string Output; //stdout + stderr
	};

	ProcessResult RunProcess(const std::string& commandLine, const std::filesystem::path& workingDirectory = {});
}//namespace Sengine::Platform