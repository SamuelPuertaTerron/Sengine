#pragma once

#include <future>

#include "Engine/Assets/AssetManager.h"
#include "Engine/Game/WorldManager.h"
#include "Engine/Platform/PlatformUtils.h"

#include "ProjectRegistry.h"

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
		enum class ETaskKind { CreateProject, InstallEngine, UpdateEngine };
		enum class ETaskState { Idle, Running, Succeeded, Failed };

		//One background command (git or premake) at a time, shown in the progress popup or the Console.
		struct BackgroundTask
		{
			ETaskKind Kind = ETaskKind::CreateProject;
			ETaskState State = ETaskState::Idle;
			std::future<Platform::ProcessResult> Future;
			std::string Command;
			std::string Name;		//Project name, for CreateProject.
			fs::path Folder;		//The project or engine folder the task works on.
			double StartTime = 0.0;
			int ExitCode = 0;
			std::string ErrorSummary;
		};

		//Drawn inside the Project Hub window.
		void DrawMenuBar();
		void DrawEngineSection();
		void DrawProjectList();
		void DrawNewProject();
		void DrawInstallPopup();
		void DrawTaskPopup();

		//Separate windows.
		void DrawOptionsWindow();
		void DrawConsoleWindow();

		void StartTask(ETaskKind kind, const std::string& name, const fs::path& folder,
			const std::string& command, const fs::path& workingDirectory);
		void PollTask();
		bool OnTaskSucceeded(); //Follow-up work. Returns false, with ErrorSummary set, if it fails.
		void UseEngine(const fs::path& root);
		bool IsBusy() const { return m_Task.State == ETaskState::Running; }

	private:
		fs::path m_InstalledEngine;	//SENGINE_ROOT: what projects build against.
		fs::path m_LocalEngine;		//The engine this hub exe was built from.
		ProjectRegistry m_Registry;

		//----- Inputs -----
		char m_NameBuffer[64] = {};
		char m_LocationBuffer[512] = {};
		char m_EngineUrlBuffer[512] = {};
		char m_InstallFolderBuffer[512] = {};

		//----- Background task -----
		BackgroundTask m_Task;
		bool m_bOpenTaskPopup = false;	//OpenPopup must run in the hub window's ID scope, so it's deferred to there.

		//----- Output -----
		std::string m_Log;
		bool m_bScrollConsoleToBottom = false;

		//----- Options -----
		bool m_bShowOptions = false;
		bool m_bDebugView = false;		//Full console output instead of the progress popup.
	};
}//namespace ProjectLoader
