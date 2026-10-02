#include "ExampleGLobals.h"
#include "Example/Exmaple.h"

namespace Example
{
	namespace
	{
		//----- World -----

		constexpr float kGravity = 9.8f;
		constexpr Raylib::Color kClearColour{ 30, 30, 46, 255 };

		//----- Rock -----

		constexpr Raylib::Vector2 kRockSpawn{ 50.0f, -200.0f };
		constexpr Raylib::Vector2 kRockScale{ 1.5f, 1.5f };
		constexpr Raylib::Color kRockTint{ 255, 255, 255, 120 };
		constexpr float kRockDensity = 0.1f;

		//----- Ground and ramps -----

		constexpr Raylib::Color kBlockColour{ 197, 0, 0, 255 };
		constexpr float kSquareTextureSize = 32.0f;				

		constexpr Raylib::Vector2 kGroundPosition{ 0.0f, 200.0f };
		constexpr Raylib::Vector2 kGroundScale{ 50.0f, 3.0f };

		constexpr float kRampScaleX = 10.0f;
		constexpr float kRampAngle = 45.0f;
		constexpr float kRampY = 120.0f;
		constexpr float kRampGap = 20.0f;						
		constexpr float kCos45 = 0.70710678f;

		//----- HUD -----

		constexpr Raylib::Vector2 kHudOrigin{ 2.0f, 2.0f };
		constexpr float kHudWidth = 300.0f;
		constexpr float kHudPadding = 6.0f;
		constexpr float kRowGap = 8.0f;
		constexpr float kLabelHeight = 20.0f;						
		constexpr float kButtonHeight = 30.0f;						
		constexpr Raylib::Color kHudBackground{ 49, 50, 68, 220 };

		//----- Helpers -----

		Raylib::Camera2D MakeCamera()
		{
			const Raylib::Vector2 virtualSize = Renderer2D::GetVirtualSize();

			Raylib::Camera2D camera{};
			camera.offset = { virtualSize.x * 0.5f, virtualSize.y * 0.5f };	
			camera.zoom = 1.0f;
			return camera;
		}

		std::string BounceLabel(int bounces)
		{
			return std::format("Bounces: {}", bounces);
		}

		//A solid, textured, static box with a collider.
		Entity CreateBlock(World& world, const std::string& name, std::shared_ptr<Texture> texture,
			Raylib::Vector2 position, Raylib::Vector2 scale, float rotation = 0.0f)
		{
			Entity block = world.CreateEntity(name);

			auto& transform = block.AddComponent<TransformComponent>();
			transform.Position = position;
			transform.Scale = scale;
			transform.Rotation = rotation;

			auto& sprite = block.AddComponent<TextureComponent>();
			sprite.Texture = std::move(texture);
			sprite.Tint = kBlockColour;
			sprite.Layer = 1;

			block.AddComponent<BoxColliderComponent>();
			return block;
		}

		Entity CreatePanel(World& world, const std::string& name, Raylib::Rectangle bounds, Raylib::Color background)
		{
			Entity panel = world.CreateEntity(name);
			panel.AddComponent<TransformComponent>().Position = { bounds.x, bounds.y };

			auto& component = panel.AddComponent<PanelComponent>();
			component.Size = { bounds.width, bounds.height };
			component.BackgroundColour = background;
			return panel;
		}

		Entity CreateLabel(World& world, const std::string& name, Raylib::Vector2 position, const std::string& text)
		{
			Entity label = world.CreateEntity(name);
			label.AddComponent<TransformComponent>().Position = position;
			label.AddComponent<TextComponent>().Text = text;
			return label;
		}

		Entity CreateButton(World& world, const std::string& name, Raylib::Vector2 position,
			const std::string& text, std::function<void()> onClicked)
		{
			Entity button = world.CreateEntity(name);
			button.AddComponent<TransformComponent>().Position = position;

			auto& component = button.AddComponent<ButtonComponent>();
			component.Text = text;
			component.ButtonClicked = std::move(onClicked);
			return button;
		}
	}//namespace

	void Example::OnCreate()
	{
		//One-time global setup, not repeated on reset.
		PhysicsSettings::Gravity = kGravity;
		GUIStyleSettings::LoadDarkTheme();

		BuildWorld();
	}

	void Example::OnTick(float deltaTime)
	{
		if (m_bResetRequested || Input::IsKeyPressed(Input::EKeyCode::R))
		{
			m_bResetRequested = false;
			ResetWorld();
		}

		if (Input::IsKeyPressed(Input::EKeyCode::Space))
		{
			Time::Resume();
		}

		Renderer2D::BeginFrame(kClearColour);
		m_World.OnTick(deltaTime);
		Renderer2D::EndFrame();
	}

	void Example::OnDestroy()
	{
		m_World.OnDestroy();
	}

	void Example::BuildWorld()
	{
		m_Bounces = 0;

		CreateRock();
		CreateGround();
		CreateHud();

		m_World.AddSystem<PhysicsSystem>();
		m_World.AddSystem<RenderSystem>(MakeCamera());
		m_World.AddSystem<AudioSystem>();
		m_World.AddSystem<UISystem>();		//After RenderSystem so the HUD draws on top.
		m_World.OnCreate();

		Time::Pause();						
	}

	void Example::ResetWorld()
	{
		m_World.OnDestroy();
		m_BounceText = {};

		BuildWorld();

		Logging::Log(Logging::ELogType::Info, "World reset");
	}

	void Example::CreateRock()
	{
		Entity rock = m_World.CreateEntity("Rock");

		auto& transform = rock.AddComponent<TransformComponent>();
		transform.Position = kRockSpawn;
		transform.Scale = kRockScale;

		auto& sprite = rock.AddComponent<TextureComponent>();
		sprite.Texture = m_Assets.GetTexture("Rock.png");
		sprite.Tint = kRockTint;
		sprite.Layer = 1;

		rock.AddComponent<AudioComponent>(m_Assets.GetAudioClip("RockImpact.wav"));
		rock.AddComponent<BoxColliderComponent>().Material.Density = kRockDensity;
		rock.AddComponent<RigidbodyComponent>().Type = RigidbodyType::DynamicBody;
#
		rock.AddComponent<CollisionCallbacksComponent>().OnCollisionEnter =
			[this](Entity self, Entity other) 
			{
				OnRockCollision(self, other); 
			};
	}

	void Example::CreateGround()
	{
		const std::shared_ptr<Texture> square = m_Assets.GetTexture("Square.png");

		CreateBlock(m_World, "Ground", square, kGroundPosition, kGroundScale);

		const float rampHalfLength = kSquareTextureSize * kRampScaleX * 0.5f;
		const float rampX = rampHalfLength * kCos45 + kRampGap * 0.5f;
		const Raylib::Vector2 rampScale{ kRampScaleX, 1.0f };

		Entity left = CreateBlock(m_World, "LeftRamp", square, { -rampX, kRampY }, rampScale, kRampAngle);
		Entity right = CreateBlock(m_World, "RightRamp", square, { rampX, kRampY }, rampScale, 180.0f - kRampAngle);

		//Fully bouncy ramps.
		left.GetComponent<BoxColliderComponent>().Material.Restitution = 1.0f;
		right.GetComponent<BoxColliderComponent>().Material.Restitution = 1.0f;
	}

	void Example::CreateHud()
	{
		//Rows are laid out top to bottom, then the panel is sized to fit them.
		const float x = kHudOrigin.x + kHudPadding;
		float y = kHudOrigin.y + kHudPadding;

		CreateLabel(m_World, "TitleText", { x, y }, "Hello from Sengine");
		y += kLabelHeight + kRowGap;

		m_BounceText = CreateLabel(m_World, "BounceText", { x, y }, BounceLabel(m_Bounces));
		y += kLabelHeight + kRowGap;

		CreateButton(m_World, "PlayButton", { x, y }, "Play", []() { Time::Resume(); });
		y += kButtonHeight + kRowGap;

		CreateButton(m_World, "ResetButton", { x, y }, "Reset", [this]() { m_bResetRequested = true; });
		y += kButtonHeight + kRowGap;

		CreateButton(m_World, "QuitButton", { x, y }, "Quit Game!", []() { Engine::Quit(); });
		const float bottom = y + kButtonHeight + kHudPadding;

		CreatePanel(m_World, "HudPanel",
			{ kHudOrigin.x, kHudOrigin.y, kHudWidth, bottom - kHudOrigin.y }, kHudBackground);
	}

	void Example::OnRockCollision(Entity self, Entity other)
	{
		++m_Bounces;

		//Only rewrite the label when the count actually changes.
		if (m_BounceText)
		{
			m_BounceText.GetComponent<TextComponent>().Text = BounceLabel(m_Bounces);
		}

		if (!self.HasComponent<PlaySoundRequestComponent>())
		{
			self.AddComponent<PlaySoundRequestComponent>();
		}

		Logging::Log(Logging::ELogType::Info, std::format("Rock hit {}",
			other.GetComponent<IdentificationComponent>().Name));
	}
}//namespace Example