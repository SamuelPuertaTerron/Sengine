#include "Globals.h"
#include "Script.h"

namespace Sengine::Scripting
{
	Script::Script(const fs::path& path)
		: m_Path(path)
	{
	}

	bool Script::IsValid() const
	{
		return !m_Path.empty() && fs::exists(m_Path);
	}

	const fs::path& Script::GetPath() const
	{
		return m_Path;
	}
}//namespace Sengine::Scripting