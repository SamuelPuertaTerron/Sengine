#include "Globals.h"
#include "Filesystem.h"

namespace Sengine::Filesystem
{
	bool Exist(const fs::path& path)
	{
		return fs::exists(path);
	}

	void SaveFile(const fs::path& path, const std::string& text)
	{
		std::ofstream file(path);
		file << text + "\n";
	}

	std::string LoadFile(const fs::path& path)
	{
		std::ifstream file(path);
		if (!file) 
		{
			throw std::runtime_error("Could not open file");
		}

		std::ostringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}
}//namespace Sengine::Filesystem