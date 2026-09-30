#pragma once

namespace Sengine::Filesystem
{
	bool Exist(const fs::path& path);

	void SaveFile(const fs::path& path, const std::string& text);
	std::string LoadFile(const fs::path& path);


}//namespace Sengine::Filesystem
