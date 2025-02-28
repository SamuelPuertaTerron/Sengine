#include "Sengine/Globals.h"
#include "Editor.h"

#include "Sengine/Platform/IPlatformContext.h"
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
		m_WindowDescription.Width = 1270;
		m_WindowDescription.Height = 720;

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
}
