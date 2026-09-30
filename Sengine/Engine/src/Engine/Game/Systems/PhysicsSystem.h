#pragma once

#include "box2d/box2d.h"
#include "entt/entt.hpp"

#include "Engine/Game/ISystem.h"

namespace Sengine
{
	struct TransformComponent;
	struct BoxColliderComponent;
	struct TextureComponent;

	inline constexpr float PixelsPerMeter = 64.0f;

	namespace Physics
	{
		[[nodiscard]] constexpr float ToMeters(float pixels) { return pixels / PixelsPerMeter; }
		[[nodiscard]] constexpr float ToPixels(float meters) { return meters * PixelsPerMeter; }

		[[nodiscard]] inline b2Vec2 ToMeters(Raylib::Vector2 pixels) { return { ToMeters(pixels.x), ToMeters(pixels.y) }; }
		[[nodiscard]] inline Raylib::Vector2 ToPixels(b2Vec2 meters) { return { ToPixels(meters.x), ToPixels(meters.y) }; }

		[[nodiscard]] Raylib::Vector2 GetColliderWorldSize(const BoxColliderComponent& collider,
			const TransformComponent& transform, const TextureComponent* texture = nullptr);

		[[nodiscard]] Raylib::Vector2 GetColliderWorldOffset(const BoxColliderComponent& collider,
			const TransformComponent& transform);
	}//namespace Physics

	class PhysicsSystem : public ISystem
	{
	public:
		void OnCreate(World& world) override;
		void OnTick(World& world, float deltaTime) override;
		void OnDestroy(World& world) override;
		void OnEnable(World& world) override;
		[[nodiscard]] std::string_view GetName() const override { return "Physics System"; }

	private:
		enum class CollisionEventType : uint8_t
		{
			CollisionEnter,
			CollisionExit,
			TriggerEnter,
			TriggerExit
		};

		struct PhysicsBody
		{
			b2BodyId Body;
			b2ShapeId Shape;

			//World-pixel size/offset the shape was last built with, so it is only rebuilt on change.
			Raylib::Vector2 Size{ 0.0f, 0.0f };
			Raylib::Vector2 Offset{ 0.0f, 0.0f };
		};

		struct PendingCollisionEvent
		{
			entt::entity A;
			entt::entity B;
			CollisionEventType Type;
		};

		void SyncEnabledState(World& world);
		void SyncMaterials(World& world);
		void SyncColliderShapes(World& world);
		void ApplyForces(World& world);
		void CollectEvents();
		void SyncTransforms(World& world);
		void DispatchEvents(World& world);
		void ApplyTeleports(World& world);

	private:
		b2WorldDef m_WorldDef;
		b2WorldId m_WorldId;

		const int m_StepCount{ 4 };

		std::unordered_map<entt::entity, PhysicsBody> m_EntityBodies;
		std::vector<PendingCollisionEvent> m_PendingEvents;
	};
}//namespace Sengine