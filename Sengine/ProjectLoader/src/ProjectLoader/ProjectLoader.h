#pragma once

#include <future>

#include "Engine/Assets/AssetManager.h"
#include "Engine/Game/WorldManager.h"

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
		std::future<Platform::ProcessResult> m_PendingCommand;
		std::string m_Log;
	};
}//namespace Editor