#include "ProjectLoaderGlobals.h"
#include "ProjectLoader.h"

#include "Engine/Game/WorldManager.h"
#include "Engine/Game/Serialization/ComponentSerialization.h"

#include "EngineLocator.h"

namespace ProjectLoader
{
	namespace
	{
		std::string Quote(const fs::path& path)
		{
			return "\"" + path.string() + "\"";
		}

		bool IsValidProjectName(const std::string& name)
		{
			if (name.empty() || std::isdigit(static_cast<unsigned char>(name[0])))
				return false;

			return std::all_of(name.begin(), name.end(), [](unsigned char c) { return std::isalnum(c) || c == '_'; });
		}
	}//namespace

	void ProjectLoader::OnCreate()
	{
		ImGuiLoader::Init(true);

		m_EngineRoot = FindEngineRoot();
		m_Registry.Load();

		const std::string documents = Platform::GetDocumentsPath().string();
		std::snprintf(m_LocationBuffer, sizeof(m_LocationBuffer), "%s", documents.c_str());
	}

	void ProjectLoader::OnTick(float deltaTime)
	{
		Renderer2D::BeginFrame(Raylib::BLACK);
		ImGuiLoader::BeginFrame();

		ImGui::Begin("Project Hub");
		{
			if (m_EngineRoot.empty())
			{
				ImGui::TextColored({ 1.0f, 0.4f, 0.4f, 1.0f }, "Engine not found. Set SENGINE_ROOT or run the hub from inside the engine repo.");
				ImGui::End();
				ImGuiLoader::EndFrame();
				Renderer2D::EndFrame();
				return;
			}

			ImGui::Text("Engine: %s", m_EngineRoot.string().c_str());

			//----- Existing projects -----
			ImGui::SeparatorText("Projects");
			if (m_Registry.GetProjects().empty())
				ImGui::TextDisabled("No projects yet.");

			for (const auto& project : m_Registry.GetProjects())
				ImGui::BulletText("%s  (%s)", project.Name.c_str(), project.Path.string().c_str());

			//----- New project -----
			ImGui::SeparatorText("New Project");
			ImGui::InputText("Name", m_NameBuffer, sizeof(m_NameBuffer));
			ImGui::InputText("Location", m_LocationBuffer, sizeof(m_LocationBuffer));

			const std::string name = m_NameBuffer;
			const fs::path location = m_LocationBuffer;
			const bool running = m_PendingCommand.valid();
			const bool exists = !name.empty() && fs::exists(location / name);
			const bool canCreate = IsValidProjectName(name) && fs::is_directory(location) && !exists && !running;

			ImGui::BeginDisabled(!canCreate);
			if (ImGui::Button("Create Project"))
				StartCreate(name);
			ImGui::EndDisabled();

			if (!name.empty() && !IsValidProjectName(name))
				ImGui::TextColored({ 1.0f, 0.4f, 0.4f, 1.0f }, "Use letters, digits and _ only, not starting with a digit.");
			else if (exists)
				ImGui::TextColored({ 1.0f, 0.4f, 0.4f, 1.0f }, "That folder already exists.");
			else if (!fs::is_directory(location))
				ImGui::TextColored({ 1.0f, 0.4f, 0.4f, 1.0f }, "Location folder does not exist.");

			//----- Collect the finished command -----
			if (running && m_PendingCommand.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				const Platform::ProcessResult result = m_PendingCommand.get();
				m_Log = result.Output;

				if (result.ExitCode == 0)
				{
					m_Registry.Add({ m_PendingName, m_PendingLocation / m_PendingName });   //<-- here
					m_NameBuffer[0] = '\0';
				}
				else
				{
					Logging::Log(Logging::ELogType::Error, std::format("premake exited with code {}: {}", result.ExitCode, result.Output));
				}
			}

			//----- Output -----
			ImGui::SeparatorText("Output");
			if (running) 
				ImGui::TextUnformatted("Running premake...");
			ImGui::TextWrapped("%s", m_Log.c_str());
		}
		ImGui::End();

		ImGuiLoader::EndFrame();
		Renderer2D::EndFrame();
	}

	void ProjectLoader::StartCreate(const std::string& name)
	{
		m_PendingName = name;
		m_PendingLocation = m_LocationBuffer;
		m_Log.clear();

		const std::string command = Quote(m_EngineRoot / "ThirdParty" / "premake" / "premake5.exe")
			+ " --file=" + Quote(m_EngineRoot / "Premake5.lua")
			+ " newproject --name=" + name
			+ " --location=" + Quote(m_PendingLocation);

		m_PendingCommand = std::async(std::launch::async, [command, root = m_EngineRoot]()
			{
				return Platform::RunProcess(command, root);
			});
	}

	void ProjectLoader::OnDestroy()
	{
	}
}//namespace ProjectLoader