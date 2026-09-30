#include "Globals.h"
#include "ImGuiLoader.h"

namespace Sengine::ImGuiLoader
{
	namespace
	{
		bool s_bShouldUseDocking = false;
	}//namespace

	void Init(bool docking)
	{
		s_bShouldUseDocking = docking;

		Raylib::ImGui::rlImGuiSetup(true);

		if(docking)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		}
	}

	void BeginFrame()
	{
		Raylib::ImGui::rlImGuiBegin();
		if (s_bShouldUseDocking)
		{
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(),
				ImGuiDockNodeFlags_PassthruCentralNode);
		}
	}

	void EndFrame()
	{
		Raylib::ImGui::rlImGuiEnd();
	}

	void Destroy()
	{
		Raylib::ImGui::rlImGuiShutdown();
	}
}//namespace Sengine::ImGuiLoader
