#include "ProjectLoaderGlobals.h"
#include "ProjectLoader.h"

#include <algorithm>
#include <cctype>
#include <cfloat>
#include <cstdlib>
#include <sstream>

#include "Engine/Platform/Dialogs.h"
#include "Engine/Platform/Environment.h"
#include "EngineLocator.h"

namespace ProjectLoader
{
	namespace
	{
		//"###" gives a popup a fixed ID, so its visible title can change without breaking OpenPopup.
		constexpr const char* kTaskPopupId = "###TaskPopup";
		constexpr const char* kInstallPopupId = "Install Engine###InstallEnginePopup";

		constexpr const char* kDefaultEngineUrl = "https://github.com/SamuelPuertaTerron/Sengine.git";

		const ImVec4 kErrorColour{ 1.0f, 0.4f, 0.4f, 1.0f };
		const ImVec4 kWarningColour{ 1.0f, 0.8f, 0.3f, 1.0f };
		const ImVec4 kSuccessColour{ 0.4f, 0.9f, 0.5f, 1.0f };

		std::string Quote(const fs::path& path)
		{
			return "\"" + path.string() + "\"";
		}

		fs::path PremakePath(const fs::path& engineRoot)
		{
#ifdef FE_PLATFORM_WINDOWS
			return engineRoot / "ThirdParty" / "premake" / "premake5.exe";
#else
			return engineRoot / "ThirdParty" / "premake" / "premake5";
#endif
		}

		fs::path DefaultInstallFolder()
		{
#ifdef FE_PLATFORM_WINDOWS
			const char* home = std::getenv("USERPROFILE");
#else
			const char* home = std::getenv("HOME");
#endif
			return home ? fs::path(home) / "Sengine" : fs::path("Sengine");
		}

		bool IsValidProjectName(const std::string& name)
		{
			if (name.empty() || std::isdigit(static_cast<unsigned char>(name[0])))
				return false;

			return std::all_of(name.begin(), name.end(), [](unsigned char c) { return std::isalnum(c) || c == '_'; });
		}

		fs::path SolutionPath(const fs::path& projectFolder, const std::string& name)
		{
			return projectFolder / (name + ".sln");
		}

		bool SamePath(const fs::path& a, const fs::path& b)
		{
			std::error_code error;
			return !a.empty() && !b.empty() && fs::equivalent(a, b, error);
		}

		//Picks the most useful line for non-debug users, e.g.
		//"Error: D:/.../NewProject.lua:42: Template folder not found" -> "Template folder not found".
		std::string SummariseError(const std::string& output)
		{
			std::string lastLine;
			std::string lastError;

			std::istringstream stream(output);
			for (std::string line; std::getline(stream, line);)
			{
				if (!line.empty() && line.back() == '\r') line.pop_back();
				if (line.find_first_not_of(" \t") == std::string::npos) continue;

				lastLine = line;
				if (line.find("Error") != std::string::npos || line.find("error") != std::string::npos || line.find("fatal") != std::string::npos)
					lastError = line;
			}

			std::string summary = lastError.empty() ? lastLine : lastError;
			if (const size_t lua = summary.find(".lua:"); lua != std::string::npos)
				if (const size_t message = summary.find(": ", lua + 5); message != std::string::npos)
					summary = summary.substr(message + 2);

			return summary.empty() ? "Unknown error." : summary;
		}

		//A small rotating arc, drawn in place of a widget.
		void Spinner(float radius, float thickness)
		{
			const ImVec2 position = ImGui::GetCursorScreenPos();
			ImGui::Dummy({ radius * 2.0f, radius * 2.0f });

			const ImVec2 centre{ position.x + radius, position.y + radius };
			const float start = static_cast<float>(ImGui::GetTime()) * 6.0f;

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->PathArcTo(centre, radius - thickness * 0.5f, start, start + 3.14159265f * 1.5f, 24);
			drawList->PathStroke(ImGui::GetColorU32(ImGuiCol_ButtonActive), 0, thickness);
		}

		void CopyToBuffer(char* buffer, size_t size, const std::string& text)
		{
			std::snprintf(buffer, size, "%s", text.c_str());
		}
	}//namespace

	void ProjectLoader::OnCreate()
	{
		ImGuiLoader::Init(true);

		m_InstalledEngine = FindInstalledEngine();
		m_LocalEngine = FindLocalEngine();
		m_Registry.Load();

		CopyToBuffer(m_LocationBuffer, sizeof(m_LocationBuffer), Platform::GetDocumentsPath().string());
		CopyToBuffer(m_EngineUrlBuffer, sizeof(m_EngineUrlBuffer), kDefaultEngineUrl);
		CopyToBuffer(m_InstallFolderBuffer, sizeof(m_InstallFolderBuffer), DefaultInstallFolder().string());
	}

	void ProjectLoader::OnTick(float deltaTime)
	{
		Renderer2D::BeginFrame(Raylib::BLACK);
		ImGuiLoader::BeginFrame();

		ImGui::Begin("Project Hub", nullptr, ImGuiWindowFlags_MenuBar);
		{
			DrawMenuBar();
			DrawEngineSection();

			//Projects build against the installed engine, so there's nothing to create without one.
			if (!m_InstalledEngine.empty())
			{
				DrawProjectList();
				DrawNewProject();
			}

			PollTask();
			if (m_bOpenTaskPopup && !m_bDebugView)
				ImGui::OpenPopup(kTaskPopupId);
			m_bOpenTaskPopup = false;

			DrawInstallPopup();
			DrawTaskPopup();
		}
		ImGui::End();

		DrawOptionsWindow();
		if (m_bDebugView)
			DrawConsoleWindow();

		ImGuiLoader::EndFrame();
		Renderer2D::EndFrame();
	}

	void ProjectLoader::OnDestroy()
	{
		//Nothing to do: if a command is still running, the std::future destructor waits for it.
	}

	//---------------------------------------------------------------------------------------------
	// Project Hub window
	//---------------------------------------------------------------------------------------------

	void ProjectLoader::DrawMenuBar()
	{
		if (!ImGui::BeginMenuBar())
			return;

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Options", nullptr, &m_bShowOptions);
			ImGui::MenuItem("Console", nullptr, &m_bDebugView);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	void ProjectLoader::DrawEngineSection()
	{
		ImGui::SeparatorText("Engine");

		if (m_InstalledEngine.empty())
		{
			ImGui::TextColored(kWarningColour, "No engine installed.");
			ImGui::TextDisabled("Projects find the engine through the %s environment variable.", kEngineVariable);
		}
		else
		{
			ImGui::TextUnformatted(m_InstalledEngine.string().c_str());
		}

		ImGui::BeginDisabled(IsBusy());

		if (ImGui::Button("Install Engine..."))
			ImGui::OpenPopup(kInstallPopupId);

		if (!m_InstalledEngine.empty())
		{
			ImGui::SameLine();
			if (ImGui::Button("Update"))
			{
				const std::string command = "git -C " + Quote(m_InstalledEngine) + " pull --ff-only";
				StartTask(ETaskKind::UpdateEngine, "Sengine", m_InstalledEngine, command, m_InstalledEngine);
			}

			ImGui::SameLine();
			if (ImGui::Button("Open Folder"))
				Platform::OpenInFileExplorer(m_InstalledEngine);
		}

		//Handy while developing the engine: point SENGINE_ROOT at your working copy instead of a separate clone.
		if (!m_LocalEngine.empty() && !SamePath(m_LocalEngine, m_InstalledEngine))
		{
			ImGui::SameLine();
			if (ImGui::Button("Use This Engine"))
				UseEngine(m_LocalEngine);

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("Set %s to the engine this hub was built from:\n%s", kEngineVariable, m_LocalEngine.string().c_str());
		}

		ImGui::EndDisabled();
	}

	void ProjectLoader::DrawProjectList()
	{
		ImGui::SeparatorText("Projects");

		const std::vector<ProjectInfo>& projects = m_Registry.GetProjects();
		if (projects.empty())
		{
			ImGui::TextDisabled("No projects yet.");
			return;
		}

		const ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp;
		if (!ImGui::BeginTable("ProjectTable", 3, flags))
			return;

		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed);

		for (size_t i = 0; i < projects.size(); ++i)
		{
			const ProjectInfo& project = projects[i];
			const fs::path solution = SolutionPath(project.Path, project.Name);

			ImGui::PushID(static_cast<int>(i));
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(project.Name.c_str());

			ImGui::TableSetColumnIndex(1);
			ImGui::AlignTextToFramePadding();
			ImGui::TextDisabled("%s", project.Path.string().c_str());

			ImGui::TableSetColumnIndex(2);
			if (ImGui::Button("Open Folder"))
				Platform::OpenInFileExplorer(project.Path);

			ImGui::SameLine();
			ImGui::BeginDisabled(!fs::exists(solution));
			if (ImGui::Button("Open Solution"))
				Platform::OpenWithDefaultApp(solution);
			ImGui::EndDisabled();

			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	void ProjectLoader::DrawNewProject()
	{
		ImGui::SeparatorText("New Project");

		ImGui::InputText("Name", m_NameBuffer, sizeof(m_NameBuffer));

		ImGui::InputText("Location", m_LocationBuffer, sizeof(m_LocationBuffer));
		ImGui::SameLine();
		if (ImGui::Button("Browse...##Location"))
		{
			if (const auto folder = Platform::PickFolder(Raylib::GetWindowHandle(), m_LocationBuffer))
				CopyToBuffer(m_LocationBuffer, sizeof(m_LocationBuffer), folder->string());
		}

		const std::string name = m_NameBuffer;
		const fs::path location = m_LocationBuffer;
		const bool locationExists = fs::is_directory(location);
		const bool projectExists = !name.empty() && fs::exists(location / name);
		const bool canCreate = IsValidProjectName(name) && locationExists && !projectExists && !IsBusy();

		ImGui::BeginDisabled(!canCreate);
		if (ImGui::Button("Create Project"))
		{
			const std::string command = Quote(PremakePath(m_InstalledEngine))
				+ " --file=" + Quote(m_InstalledEngine / "Premake5.lua")
				+ " newproject --name=" + name
				+ " --location=" + Quote(location);

			StartTask(ETaskKind::CreateProject, name, location / name, command, m_InstalledEngine);
		}
		ImGui::EndDisabled();

		if (!name.empty() && !IsValidProjectName(name))
			ImGui::TextColored(kErrorColour, "Use letters, digits and _ only, not starting with a digit.");
		else if (projectExists)
			ImGui::TextColored(kErrorColour, "That folder already exists.");
		else if (!locationExists)
			ImGui::TextColored(kErrorColour, "Location folder does not exist.");
		else if (IsBusy() && m_bDebugView)
			ImGui::TextDisabled("Running... see the Console window.");
	}

	void ProjectLoader::DrawInstallPopup()
	{
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, { 0.5f, 0.5f });
		if (!ImGui::BeginPopupModal(kInstallPopupId, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			return;

		ImGui::TextUnformatted("Downloads the engine once. Every project then builds against this copy.");
		ImGui::Spacing();

		ImGui::InputText("Repository", m_EngineUrlBuffer, sizeof(m_EngineUrlBuffer));

		ImGui::InputText("Folder", m_InstallFolderBuffer, sizeof(m_InstallFolderBuffer));
		ImGui::SameLine();
		if (ImGui::Button("Browse...##Install"))
		{
			//Pick where to put it; the engine goes in a "Sengine" folder inside.
			const fs::path current = m_InstallFolderBuffer;
			if (const auto folder = Platform::PickFolder(Raylib::GetWindowHandle(), current.parent_path()))
				CopyToBuffer(m_InstallFolderBuffer, sizeof(m_InstallFolderBuffer), (*folder / "Sengine").string());
		}

		const std::string url = m_EngineUrlBuffer;
		const fs::path folder = m_InstallFolderBuffer;

		std::error_code error;
		const bool urlValid = !url.empty() && url.find('"') == std::string::npos;
		const bool parentExists = folder.has_parent_path() && fs::is_directory(folder.parent_path(), error);
		const bool folderFree = !fs::exists(folder, error) || (fs::is_directory(folder, error) && fs::is_empty(folder, error));

		if (!urlValid)
			ImGui::TextColored(kErrorColour, "Enter the engine's git repository URL.");
		else if (!parentExists)
			ImGui::TextColored(kErrorColour, "The folder above it does not exist.");
		else if (!folderFree)
			ImGui::TextColored(kErrorColour, "That folder already exists and isn't empty.");
		else
			ImGui::TextDisabled("Needs git installed. %s will point to this folder.", kEngineVariable);

		ImGui::Spacing();
		ImGui::BeginDisabled(!(urlValid && parentExists && folderFree) || IsBusy());
		if (ImGui::Button("Install"))
		{
			const std::string command = "git clone --recursive \"" + url + "\" " + Quote(folder);
			StartTask(ETaskKind::InstallEngine, "Sengine", folder, command, folder.parent_path());
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	void ProjectLoader::DrawTaskPopup()
	{
		const char* heading = "New Project";
		if (m_Task.Kind == ETaskKind::InstallEngine) heading = "Install Engine";
		if (m_Task.Kind == ETaskKind::UpdateEngine) heading = "Update Engine";
		const std::string title = std::string(heading) + kTaskPopupId;

		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, { 0.5f, 0.5f });
		ImGui::SetNextWindowSizeConstraints({ 380.0f, 0.0f }, { 600.0f, FLT_MAX });

		//No close button: while running it can't be dismissed, afterwards the buttons close it.
		if (!ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
			return;

		bool close = false;

		switch (m_Task.State)
		{
		case ETaskState::Running:
		{
			Spinner(9.0f, 3.0f);
			ImGui::SameLine();
			ImGui::AlignTextToFramePadding();

			const double elapsed = ImGui::GetTime() - m_Task.StartTime;
			switch (m_Task.Kind)
			{
			case ETaskKind::CreateProject:
				ImGui::Text("Creating Project %s...", m_Task.Name.c_str());
				ImGui::Spacing();
				ImGui::TextDisabled("Copying templates and generating the solution. (%.0fs)", elapsed);
				break;
			case ETaskKind::InstallEngine:
				ImGui::TextUnformatted("Installing Sengine...");
				ImGui::Spacing();
				ImGui::TextDisabled("Downloading the engine can take a few minutes. (%.0fs)", elapsed);
				break;
			case ETaskKind::UpdateEngine:
				ImGui::TextUnformatted("Updating Sengine...");
				ImGui::Spacing();
				ImGui::TextDisabled("Downloading the latest changes. (%.0fs)", elapsed);
				break;
			}
			break;
		}

		case ETaskState::Succeeded:
		{
			switch (m_Task.Kind)
			{
			case ETaskKind::CreateProject:
			{
				ImGui::TextColored(kSuccessColour, "Project %s created.", m_Task.Name.c_str());
				ImGui::TextDisabled("%s", m_Task.Folder.string().c_str());
				ImGui::Spacing();

				if (ImGui::Button("Open Folder"))
					Platform::OpenInFileExplorer(m_Task.Folder);

				const fs::path solution = SolutionPath(m_Task.Folder, m_Task.Name);
				ImGui::SameLine();
				ImGui::BeginDisabled(!fs::exists(solution));
				if (ImGui::Button("Open Solution"))
					Platform::OpenWithDefaultApp(solution);
				ImGui::EndDisabled();
				ImGui::SameLine();
				break;
			}
			case ETaskKind::InstallEngine:
				ImGui::TextColored(kSuccessColour, "Sengine installed.");
				ImGui::TextDisabled("%s", m_Task.Folder.string().c_str());
				ImGui::TextDisabled("New projects will use this engine. Restart Visual Studio and\nopen terminals so they pick up %s.", kEngineVariable);
				ImGui::Spacing();

				if (ImGui::Button("Open Folder"))
					Platform::OpenInFileExplorer(m_Task.Folder);
				ImGui::SameLine();
				break;
			case ETaskKind::UpdateEngine:
				ImGui::TextColored(kSuccessColour, "Sengine is up to date.");
				ImGui::TextDisabled("Run GenerateProjects in your games if engine files were added or removed.");
				ImGui::Spacing();
				break;
			}

			if (ImGui::Button("Close"))
				close = true;
			break;
		}

		case ETaskState::Failed:
		{
			switch (m_Task.Kind)
			{
			case ETaskKind::CreateProject: ImGui::TextColored(kErrorColour, "Could not create %s.", m_Task.Name.c_str()); break;
			case ETaskKind::InstallEngine: ImGui::TextColored(kErrorColour, "Could not install Sengine."); break;
			case ETaskKind::UpdateEngine:  ImGui::TextColored(kErrorColour, "Could not update Sengine."); break;
			}

			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 520.0f);
			ImGui::TextUnformatted(m_Task.ErrorSummary.c_str());
			ImGui::PopTextWrapPos();
			ImGui::Spacing();

			if (ImGui::Button("Show Details"))
			{
				m_bDebugView = true; //Opens the Console window with the full output.
				close = true;
			}

			ImGui::SameLine();
			if (ImGui::Button("Close"))
				close = true;
			break;
		}

		case ETaskState::Idle:
			close = true;
			break;
		}

		if (close)
		{
			m_Task.State = ETaskState::Idle;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	//---------------------------------------------------------------------------------------------
	// Options and Console windows
	//---------------------------------------------------------------------------------------------

	void ProjectLoader::DrawOptionsWindow()
	{
		if (!m_bShowOptions)
			return;

		ImGui::SetNextWindowSize({ 440.0f, 0.0f }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Options", &m_bShowOptions))
		{
			ImGui::SeparatorText("Output");
			ImGui::Checkbox("Debug view", &m_bDebugView);
			ImGui::TextDisabled("Shows the full git and premake output in a Console window\ninstead of the progress popup.");

			ImGui::SeparatorText("Engine");
			ImGui::Text("%s:", kEngineVariable);
			ImGui::SameLine();
			ImGui::TextUnformatted(m_InstalledEngine.empty() ? "Not set" : m_InstalledEngine.string().c_str());

			ImGui::Text("This hub's engine:");
			ImGui::SameLine();
			ImGui::TextUnformatted(m_LocalEngine.empty() ? "Not found" : m_LocalEngine.string().c_str());
		}
		ImGui::End();
	}

	void ProjectLoader::DrawConsoleWindow()
	{
		ImGui::SetNextWindowSize({ 720.0f, 420.0f }, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Console", &m_bDebugView))
		{
			if (ImGui::Button("Copy"))
				ImGui::SetClipboardText(m_Log.c_str());

			ImGui::SameLine();
			if (ImGui::Button("Clear"))
				m_Log.clear();

			ImGui::SameLine();
			switch (m_Task.State)
			{
			case ETaskState::Running:
				ImGui::Text("Running... (%.0fs)", ImGui::GetTime() - m_Task.StartTime);
				break;
			case ETaskState::Succeeded:
				ImGui::TextColored(kSuccessColour, "Finished (exit code %d)", m_Task.ExitCode);
				break;
			case ETaskState::Failed:
				ImGui::TextColored(kErrorColour, "Failed (exit code %d): %s", m_Task.ExitCode, m_Task.ErrorSummary.c_str());
				break;
			case ETaskState::Idle:
				ImGui::TextDisabled("Idle");
				break;
			}

			if (!m_Task.Command.empty())
				ImGui::TextDisabled("> %s", m_Task.Command.c_str());

			ImGui::Separator();

			ImGui::BeginChild("ConsoleOutput", { 0.0f, 0.0f }, true, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::TextUnformatted(m_Log.data(), m_Log.data() + m_Log.size());
			if (m_bScrollConsoleToBottom)
			{
				ImGui::SetScrollHereY(1.0f);
				m_bScrollConsoleToBottom = false;
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	//---------------------------------------------------------------------------------------------
	// Background tasks
	//---------------------------------------------------------------------------------------------

	void ProjectLoader::StartTask(ETaskKind kind, const std::string& name, const fs::path& folder,
		const std::string& command, const fs::path& workingDirectory)
	{
		m_Task.Kind = kind;
		m_Task.State = ETaskState::Running;
		m_Task.Name = name;
		m_Task.Folder = folder;
		m_Task.Command = command;
		m_Task.StartTime = ImGui::GetTime();
		m_Task.ExitCode = 0;
		m_Task.ErrorSummary.clear();
		m_Log.clear();

		//Copies only: the worker thread must not read members the UI thread can change.
		m_Task.Future = std::async(std::launch::async, [command, workingDirectory]()
		{
			return Platform::RunProcess(command, workingDirectory);
		});

		m_bOpenTaskPopup = true;
	}

	void ProjectLoader::PollTask()
	{
		if (m_Task.State != ETaskState::Running || !m_Task.Future.valid())
			return;

		if (m_Task.Future.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
			return;

		const Platform::ProcessResult result = m_Task.Future.get();
		m_Log = result.Output;
		m_Task.ExitCode = result.ExitCode;
		m_bScrollConsoleToBottom = true;

		if (result.Started && result.ExitCode == 0 && OnTaskSucceeded())
		{
			m_Task.State = ETaskState::Succeeded;
		}
		else
		{
			m_Task.State = ETaskState::Failed;
			if (m_Task.ErrorSummary.empty())
				m_Task.ErrorSummary = SummariseError(result.Output);

			Logging::Log(Logging::ELogType::Error, std::format("{} failed (exit code {}): {}\n{}",
				m_Task.Command, result.ExitCode, m_Task.ErrorSummary, result.Output));
		}

		//Debug view may have been turned off while it ran: make sure the result is still shown.
		if (!ImGui::IsPopupOpen(kTaskPopupId))
			m_bOpenTaskPopup = true;
	}

	bool ProjectLoader::OnTaskSucceeded()
	{
		switch (m_Task.Kind)
		{
		case ETaskKind::CreateProject:
			m_Registry.Add({ m_Task.Name, m_Task.Folder });
			m_NameBuffer[0] = '\0';
			return true;

		case ETaskKind::InstallEngine:
			if (!IsEngineRoot(m_Task.Folder))
			{
				m_Task.ErrorSummary = "The download finished, but it isn't a Sengine engine (no Sengine.lua or Sengine/Templates).";
				return false;
			}
			if (!Platform::SetPersistentEnvironmentVariable(kEngineVariable, m_Task.Folder.string()))
			{
				m_Task.ErrorSummary = std::string("The engine was downloaded, but ") + kEngineVariable + " could not be set.";
				return false;
			}
			m_InstalledEngine = m_Task.Folder;
			return true;

		case ETaskKind::UpdateEngine:
			return true;
		}
		return true;
	}

	void ProjectLoader::UseEngine(const fs::path& root)
	{
		if (!Platform::SetPersistentEnvironmentVariable(kEngineVariable, root.string()))
		{
			Platform::ShowMessageBox(Raylib::GetWindowHandle(), "Sengine",
				std::string("Could not set ") + kEngineVariable + ".", Platform::EMessageBoxType::Error);
			return;
		}

		m_InstalledEngine = root;
		Logging::Log(Logging::ELogType::Info, std::format("{} set to {}", kEngineVariable, root.string()));
	}
}//namespace ProjectLoader
