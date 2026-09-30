#include "EditorGlobals.h"
#include "Editor.h"

#include "Engine/Game/WorldManager.h"
#include "Engine/Game/Serialization/ComponentSerialization.h"

namespace Editor
{
	static constexpr std::string_view k_SaveAsPopupId = "Save World As";
	static constexpr std::string_view k_DefaultWorldPath = "Resources/Worlds/NewWorld.json";
	static constexpr std::string_view k_WorldName = "MainLevel";

	Editor::Editor()
		: m_WorldManager(m_AssetManager)
	{
	}

	void Editor::OnCreate()
	{
		Sengine::Serialization::RegisterComponentSerializers();
		ImGuiLoader::Init(true);

		Raylib::Camera2D camera{};
		const Raylib::Vector2 virtualSize = Renderer2D::GetVirtualSize();
		camera.offset = Raylib::Vector2(virtualSize.x * 0.5f, virtualSize.y * 0.5f);
		camera.zoom = 1.0f;

		m_WorldManager.Register(k_WorldName.data(), [this, camera]()
			{
				auto world = std::make_unique<World>();

				//Creates the falling object
				{
					auto fall = world->CreateEntity("Falling");

					auto& fallTransform = fall.AddComponent<TransformComponent>();
					fallTransform.Position = { 50.0f, -200.0f };
					fallTransform.Scale = { 1.5f, 1.5f };

					auto& fallSprite = fall.AddComponent<TextureComponent>();
					fallSprite.Texture = m_AssetManager.GetTexture("Rock.png");
					fallSprite.Tint = Raylib::Color(255, 255, 255, 120);
					fallSprite.Layer = 1;

					auto& fallCollider = fall.AddComponent<BoxColliderComponent>();
					fallCollider.Trigger = false;
					fallCollider.Material.Density = 0.1f;

					auto& fallRigidbody = fall.AddComponent<RigidbodyComponent>();
					fallRigidbody.Type = RigidbodyType::DynamicBody;
					fallRigidbody.UseGravity = true;
				}
				//Creates the ground object
				{
					auto ground = world->CreateEntity("Ground");

					auto& groundTransform = ground.AddComponent<TransformComponent>();
					groundTransform.Position = { 0.0f, 200.0f };
					groundTransform.Scale = { 50.0f, 3.0f };

					auto& groundSprite = ground.AddComponent<TextureComponent>();
					groundSprite.Texture = m_AssetManager.GetTexture("Square.png");
					groundSprite.Tint = Raylib::Color(197, 0, 0, 255);
					groundSprite.Layer = 1;

					auto& groundCollider = ground.AddComponent<BoxColliderComponent>();
					groundCollider.Trigger = false;
				}

				world->AddSystem<PhysicsSystem>();
				world->AddSystem<RenderSystem>(camera);
				return world;
			});

		m_WorldManager.Request(k_WorldName.data());
	}

	void Editor::OnTick(float deltaTime)
	{
		Renderer2D::BeginFrame(Raylib::SKYBLUE);
		m_WorldManager.Tick(deltaTime);
		Renderer2D::Present();

		ImGuiLoader::BeginFrame();

		const bool hasWorld = m_WorldManager.GetActive() != nullptr;

		const ImGuiIO& io = ImGui::GetIO();
		if (hasWorld && io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S, false))
		{
			SaveWorld();
		}

		ImGui::BeginMainMenuBar();
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Create World"))
				{

				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save", "Ctrl+S", false, hasWorld))
				{
					SaveWorld();
				}

				if (ImGui::MenuItem("Save As...", nullptr, false, hasWorld))
				{
					m_bOpenSaveAsPopup = true;
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Quit"))
				{
					Engine::Quit();
				}

				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		DrawSaveAsPopup();

		ImGui::Begin("Inspector");
		{
			if (ImGui::BeginPopupContextWindow("MyContextMenu", ImGuiPopupFlags_MouseButtonRight))
			{
				if (ImGui::MenuItem("Option 1"))
				{
					// Action for Option 1
				}
				if (ImGui::MenuItem("Option 2"))
				{
					// Action for Option 2
				}
				ImGui::EndPopup();
			}
		}
		ImGui::End();

		ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		const ImVec2 pos = ImGui::GetCursorScreenPos();
		const ImVec2 avail = ImGui::GetContentRegionAvail();
		Renderer2D::SetHostRect(Raylib::Rectangle(pos.x, pos.y, avail.x, avail.y));

		const Raylib::Rectangle viewport = Renderer2D::GetViewport();
		const Raylib::Texture& canvas = Renderer2D::GetCanvasTexture();

		ImGui::SetCursorScreenPos(ImVec2(viewport.x, viewport.y));

		Raylib::ImGui::rlImGuiImageRect(&canvas,
			static_cast<int>(viewport.width), static_cast<int>(viewport.height),
			Raylib::Rectangle(0.0f, 0.0f,
				static_cast<float>(canvas.width),
				-static_cast<float>(canvas.height)));

		m_bViewportHovered = ImGui::IsItemHovered();

		ImGui::End();
		ImGuiLoader::EndFrame();

		Renderer2D::EndFrame();
	}

	void Editor::OnDestroy()
	{
		m_WorldManager.Destroy();
	}

	void Editor::SaveWorld()
	{
		if (m_WorldManager.GetActiveFile().empty())
		{
			m_bOpenSaveAsPopup = true;
			return;
		}

		m_WorldManager.SaveActive();
	}

	void Editor::DrawSaveAsPopup()
	{
		if (m_bOpenSaveAsPopup)
		{
			m_bOpenSaveAsPopup = false;
			m_bSaveAsFailed = false;

			const fs::path& current = m_WorldManager.GetActiveFile();
			const std::string initial = current.empty() ? k_DefaultWorldPath.data() : current.generic_string();
			std::snprintf(m_SaveAsPath.data(), m_SaveAsPath.size(), "%s", initial.c_str());

			ImGui::OpenPopup(k_SaveAsPopupId.data());
		}

		if (ImGui::BeginPopupModal(k_SaveAsPopupId.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::SetNextItemWidth(400.0f);
			const bool submitted = ImGui::InputText("##Path", m_SaveAsPath.data(), m_SaveAsPath.size(),
				ImGuiInputTextFlags_EnterReturnsTrue);

			if (m_bSaveAsFailed)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Save failed, see the log.");
			}

			if (ImGui::Button("Save") || submitted)
			{
				if (m_WorldManager.SaveActiveAs(m_SaveAsPath.data()))
				{
					ImGui::CloseCurrentPopup();
				}
				else
				{
					m_bSaveAsFailed = true;
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape, false))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}//namespace Editor