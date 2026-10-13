#pragma once

namespace ProjectLoader
{
	//Game projects find the shared engine install through this environment variable.
	inline constexpr const char* kEngineVariable = "SENGINE_ROOT";

	//True if dir is an engine repo root (has Sengine.lua and the project templates).
	bool IsEngineRoot(const std::filesystem::path& dir);

	//The shared install that projects build against (SENGINE_ROOT). Empty if unset or not an engine.
	std::filesystem::path FindInstalledEngine();

	//The engine this hub exe was built from, found by walking up from the exe. Empty if not found.
	std::filesystem::path FindLocalEngine();
}//namespace ProjectLoader
