#pragma once
#include <optional>
#include <string>

namespace Sengine::Platform
{
	//Reads a user environment variable from where it is saved (the registry on Windows, ~/.profile on Linux),
	//so it sees changes made after this program started. Falls back to this process's environment.
	std::optional<std::string> GetPersistentEnvironmentVariable(const std::string& name);

	//Saves a user environment variable permanently (no admin rights needed) and also sets it for this
	//process, so programs started from here see it straight away.
	bool SetPersistentEnvironmentVariable(const std::string& name, const std::string& value);
}//namespace Sengine::Platform
