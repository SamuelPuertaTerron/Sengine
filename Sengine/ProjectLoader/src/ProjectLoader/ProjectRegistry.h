#pragma once

namespace ProjectLoader
{
	struct ProjectInfo
	{
		std::string Name;
		std::filesystem::path Path;
	};

	class ProjectRegistry
	{
	public:
		void Load();        //Reads the list and drops projects whose folders no longer exist.
		void Save() const;

		void Add(ProjectInfo project);
		const std::vector<ProjectInfo>& GetProjects() const { return m_Projects; }

	private:
		std::vector<ProjectInfo> m_Projects;
	};
}//namespace ProjectLoader