
#include "Sengine/Sengine.h"
#include "Sengine/Render/Renderer.h"

namespace SengineEditor
{
	using namespace Sengine;

	class Editor : public ISengineApp
	{
	public:
		Editor() = default;
		virtual ~Editor() override = default;

		[[nodiscard]] virtual WindowDescription& GetWindowDescription() override;
		[[nodiscard]] virtual bool OnEarlyInit() override;
		[[nodiscard]] virtual bool OnInit() override;
		virtual void OnTick() override;
		virtual void OnDestroy() override;
		virtual void OnLateDestroy() override;

	private:
		WindowDescription m_WindowDescription;
	};

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
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glClearColor(0.25f, 0.6f, 0.75f, 1.0f);

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

int main()
{
	Sengine::Application app;
	std::shared_ptr<SengineEditor::Editor> editor = std::make_shared<SengineEditor::Editor>();
	app.CreateApplication(editor);
	return 0;
}