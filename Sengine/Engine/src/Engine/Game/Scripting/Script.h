#pragma once

namespace Sengine::Scripting
{
	class Script
	{
	public:
		explicit Script(const fs::path& path);
		~Script() = default;

		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] const fs::path& GetPath() const;

	private:
		fs::path m_Path;
	};
}//namespace Sengine::Scripting