#include "ProjectLoaderGlobals.h"
#include "ProjectLoader/ProjectRegistry.h"

#include "nlohmann/json.hpp"

namespace ProjectLoader
{
	namespace
	{
		fs::path GetProjectListPath()
		{
#ifdef FE_PLATFORM_WINDOWS
			const char* base = std::getenv("APPDATA");
			return base ? fs::path(base) / "Sengine" / "Projects.json" : fs::path{};
#else
			const char* base = std::getenv("HOME");
			return base ? fs::path(base) / ".config" / "Sengine" / "Projects.json" : fs::path{};
#endif
		}
	}//namespace

	void ProjectRegistry::Load()
	{
		m_Projects.clear();

		std::ifstream file(GetProjectListPath());
		if (!file) return; //No list yet: first run.

		const nlohmann::json list = nlohmann::json::parse(file, nullptr, /*allow_exceptions*/ false);
		if (!list.is_array()) return; //Missing or corrupt file: start empty rather than crash.

		for (const auto& entry : list)
		{
			ProjectInfo project{ entry.value("name", ""), entry.value("path", "") };
			if (!project.Name.empty() && fs::is_directory(project.Path))
				m_Projects.push_back(std::move(project));
		}
	}

	void ProjectRegistry::Save() const
	{
		const fs::path path = GetProjectListPath();
		if (path.empty()) return;

		nlohmann::json list = nlohmann::json::array();
		for (const auto& project : m_Projects)
			list.push_back({ { "name", project.Name }, { "path", project.Path.string() } });

		fs::create_directories(path.parent_path());
		std::ofstream(path) << list.dump(4);
	}

	void ProjectRegistry::Add(ProjectInfo project)
	{
		m_Projects.push_back(std::move(project));
		Save();
	}
}//namespace ProjectLoader