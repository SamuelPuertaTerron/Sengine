#include "ProjectLoaderGlobals.h"
#include "EngineLocator.h"

#include "Engine/Platform/Environment.h"
#include "Engine/Platform/PlatformUtils.h"

namespace ProjectLoader
{
	bool IsEngineRoot(const fs::path& dir)
	{
		std::error_code error;
		return !dir.empty()
			&& fs::exists(dir / "Sengine.lua", error)
			&& fs::is_directory(dir / "Sengine" / "Templates", error);
	}

	fs::path FindInstalledEngine()
	{
		//Read the saved value rather than this process's copy: it may have changed after the hub started.
		const std::optional<std::string> value = Platform::GetPersistentEnvironmentVariable(kEngineVariable);
		if (!value || !IsEngineRoot(*value))
			return {};

		return fs::path(*value).lexically_normal();
	}

	fs::path FindLocalEngine()
	{
		for (fs::path dir = Platform::GetExecutablePath().parent_path(); !dir.empty(); dir = dir.parent_path())
		{
			if (IsEngineRoot(dir)) return dir;
			if (IsEngineRoot(dir / "Sengine")) return dir / "Sengine";
			if (dir == dir.parent_path()) break; //Reached the drive root.
		}
		return {};
	}
}//namespace ProjectLoader
