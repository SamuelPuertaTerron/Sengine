#include "ExampleGLobals.h"
#include "Example/Exmaple.h"

namespace Example
{
	namespace
	{
		constexpr float kSquareTextureSize = 32.0f;
		constexpr float kSideScaleX = 10.0f;
		constexpr float kSideAngle = 45.0f;
		constexpr float kCentreGap = 20.0f; 
		constexpr float kSideY = 120.0f;
		constexpr float kCos45 = 0.70710678f;
	}

	void Example::OnCreate()
	{
		PhysicsSettings::Gravity = 9.8f;

		Raylib::Camera2D camera{};
		const Raylib::Vector2 virtualSize = Renderer2D::GetVirtualSize();
		camera.offset = Raylib::Vector2(virtualSize.x * 0.5f, virtualSize.y * 0.5f);
		camera.zoom = 1.0f;

		CreateFallingObject();
		CreateGroundObject();

		m_World.AddSystem<PhysicsSystem>();
		m_World.AddSystem<RenderSystem>(camera);

		m_World.OnCreate();

		Time::Pause();
	}

	void Example::OnTick(float deltaTime)
	{
		if(Input::IsKeyDown(Input::EKeyCode::Space))
		{
			Time::Resume();
		}

		Renderer2D::BeginFrame(Raylib::Color(30, 30, 46, 255));
			m_World.OnTick(deltaTime);
			Renderer2D::Present();
		Renderer2D::EndFrame();
	}

	void Example::OnDestroy()
	{
		m_World.OnDestroy();
	}

	void Example::CreateFallingObject()
	{
		auto fall = m_World.CreateEntity("Falling");

		auto& fallTransform = fall.AddComponent<TransformComponent>();
		fallTransform.Position = { 50.0f, -200.0f };
		fallTransform.Scale = { 1.5f, 1.5f };

		auto& fallSprite = fall.AddComponent<TextureComponent>();
		fallSprite.Texture = m_Assets.GetTexture("Rock.png");
		fallSprite.Tint = Raylib::Color(255, 255, 255, 120);
		fallSprite.Layer = 1;

		auto& fallCollider = fall.AddComponent<BoxColliderComponent>();
		fallCollider.Trigger = false;
		fallCollider.Material.Density = 0.1f;

		auto& fallRigidbody = fall.AddComponent<RigidbodyComponent>();
		fallRigidbody.Type = RigidbodyType::DynamicBody;
		fallRigidbody.UseGravity = true;
	}

	void Example::CreateGroundObject()
	{
		const Raylib::Color objectColour = Raylib::Color(197, 0, 0, 255);

		auto ground = m_World.CreateEntity("Ground");

		auto& groundTransform = ground.AddComponent<TransformComponent>();
		groundTransform.Position = { 0.0f, 200.0f };
		groundTransform.Scale = { 50.0f, 3.0f };

		auto& groundSprite = ground.AddComponent<TextureComponent>();
		groundSprite.Texture = m_Assets.GetTexture("Square.png");
		groundSprite.Tint = objectColour;
		groundSprite.Layer = 1;

		auto& groundCollider = ground.AddComponent<BoxColliderComponent>();
		groundCollider.Trigger = false;

		const float sideHalfLength = kSquareTextureSize * kSideScaleX * 0.5f;
		const float sideHalfSpanX = sideHalfLength * kCos45;

		const float sideX = sideHalfSpanX + kCentreGap * 0.5f;

		auto createSide = [&](const char* name, float x, float rotation)
			{
				auto side = m_World.CreateEntity(name);

				auto& transform = side.AddComponent<TransformComponent>();
				transform.Position = { x, kSideY };
				transform.Rotation = rotation;
				transform.Scale = { kSideScaleX, 1.0f };

				auto& sprite = side.AddComponent<TextureComponent>();
				sprite.Texture = m_Assets.GetTexture("Square.png");
				sprite.Tint = objectColour;
				sprite.Layer = 1;

				auto& collider = side.AddComponent<BoxColliderComponent>();
				collider.Trigger = false;
				collider.Material.Restitution = 1.0f;
			};

		createSide("LSide", -sideX, kSideAngle);           
		createSide("RSide", sideX, 180.0f - kSideAngle);
	}
}//namespace Example