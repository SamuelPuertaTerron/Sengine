#include "Sengine/Globals.h"
#include "Editor.h"

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
		Renderer::BeginRender2D({});

		Renderer::Draw2D();

		Renderer::EndRender2D();
	}
	void Editor::OnDestroy()
	{
	}
	void Editor::OnLateDestroy()
	{
	}
}