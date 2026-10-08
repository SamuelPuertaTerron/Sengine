#include "ProjectLoaderGlobals.h"
#include "ProjectLoader.h"

#include "Engine/Game/WorldManager.h"
#include "Engine/Game/Serialization/ComponentSerialization.h"

namespace ProjectLoader
{
	void ProjectLoader::OnCreate()
	{
		ImGuiLoader::Init(true);
	}

	void ProjectLoader::OnTick(float deltaTime)
	{
		Renderer2D::BeginFrame(Raylib::BLACK);
		ImGuiLoader::BeginFrame();

		ImGui::Begin("Project Hub");
		{
			const bool running = m_PendingCommand.valid();

			ImGui::BeginDisabled(running);
			if (ImGui::Button("Create Test Project"))
			{
				m_PendingCommand = std::async(std::launch::async, []()
					{
						const fs::path root = "D:/Work/Code/C++/Game Engine/Sengine";
						const fs::path premake = root / "ThirdParty/premake/premake5.exe";

						const std::string command = "\"" + premake.string() + "\" newproject --name=Test";
						return Platform::RunProcess(command, root);
					});
			}
			ImGui::EndDisabled();

			if (running && m_PendingCommand.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				const Platform::ProcessResult result = m_PendingCommand.get();
				m_Log = result.Output;
				if (result.ExitCode != 0)
					Logging::Log(Logging::ELogType::Error, std::format("premake exited with code {}: {}", result.ExitCode, result.Output));
			}

			if (running) ImGui::TextUnformatted("Running premake...");
			ImGui::TextWrapped("%s", m_Log.c_str());

			ImGui::End();
		}

		ImGuiLoader::EndFrame();
		Renderer2D::EndFrame();
	}

	void ProjectLoader::OnDestroy()
	{

	}
}//namespace ProjectLoader