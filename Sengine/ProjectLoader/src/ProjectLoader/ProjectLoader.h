#pragma once

#include <future>

#include "Engine/Assets/AssetManager.h"
#include "Engine/Game/WorldManager.h"

#include "ProjectLoader/ProjectRegistry.h"

namespace ProjectLoader
{
	class ProjectLoader : public ILayer
	{
	public:
		ProjectLoader() = default;
		~ProjectLoader() override = default;

		ProjectLoader(const ProjectLoader&) = delete;
		ProjectLoader& operator=(const ProjectLoader&) = delete;

		void OnCreate() override;
		void OnTick(float deltaTime) override;
		void OnDestroy() override;

	private:
		void StartCreate(const std::string& name);

	private:
		std::future<Platform::ProcessResult> m_PendingCommand;
		std::string m_Log;

		fs::path m_EngineRoot;
		ProjectRegistry m_Registry;

		char m_NameBuffer[64] = {};
		char m_LocationBuffer[512] = {};
		std::string m_PendingName;     //Captured when Create is pressed.
		fs::path m_PendingLocation;    //So editing the box mid-run can't change where it's registered.
	};
}//namespace Editor