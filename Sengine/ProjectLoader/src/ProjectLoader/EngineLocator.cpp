#include "ProjectLoaderGlobals.h"
#include "ProjectLoader/EngineLocator.h"

namespace ProjectLoader
{
	namespace
	{
		bool IsEngineRoot(const fs::path& dir)
		{
			return fs::exists(dir / "Sengine.lua") && fs::is_directory(dir / "Sengine" / "Templates");
		}
	}//namespace

	fs::path FindEngineRoot()
	{
		//Manual override, handy if the exe is ever installed somewhere else.
		if (const char* env = std::getenv("SENGINE_ROOT"); env && IsEngineRoot(env))
			return env;

		for (fs::path dir = Platform::GetExecutablePath().parent_path(); !dir.empty(); dir = dir.parent_path())
		{
			if (IsEngineRoot(dir)) return dir;
			if (IsEngineRoot(dir / "Sengine")) return dir / "Sengine";
			if (dir == dir.parent_path()) break; //Reached the drive root.
		}
		return {};
	}
}//namespace ProjectLoader