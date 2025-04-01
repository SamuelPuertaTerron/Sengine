#include "Sengine/Globals.h"
#include "Editor.h"

#include "Sengine/Render/Renderer.h"
#include "Sengine/Render/Resources/Camera/Camera.h"

namespace SengineEditor
{

	WindowDescription& Editor::GetWindowDescription()
	{
		return m_WindowDescription;
	}
	bool Editor::OnEarlyInit()
	{
		m_WindowDescription.Title = "Editor";
		m_WindowDescription.Size = { 1270, 720 };
		m_WindowDescription.RenderContextType = ERenderContextType::OpenGL;

		return true;
	}
	bool Editor::OnInit()
	{
		return true;
	}
	void Editor::OnTick()
	{
		Camera camera{};
		Renderer::BeginRender(camera);

		Renderer::BeginRender2D();

		Renderer::Draw2D();

		Renderer::EndRender2D();

		Renderer::EndRender();
	}
	void Editor::OnDestroy()
	{
	}
	void Editor::OnLateDestroy()
	{
	}

	void PlayerSerializer::SerializeData(nlohmann::json& out)
	{
		out["Name"] = m_Player.Name;
	}

	void PlayerSerializer::DeserializeData(nlohmann::json& in)
	{
		Logger::Log("Json File Loaded: " + in.dump(), ELogType::Debug, ELogAreaType::Generic);
	}
}
