#include "Globals.h"
#include "PhysicsSystem.h"

#include "Engine/Core/Time.h"

#include "Engine/Game/World.h"
#include "Engine/Game/Entity.h"
#include "Engine/Game/Components.h"
#include "Engine/Game/Settings.h"


namespace Sengine
{
	namespace Physics
	{
		namespace
		{
			constexpr float MinColliderSizePixels = 1.0f;
		}//namespace

		Raylib::Vector2 GetColliderWorldSize(const BoxColliderComponent& collider,
			const TransformComponent& transform, const TextureComponent* texture)
		{
			Raylib::Vector2 local = collider.Size;

			if (local.x == 0.0f || local.y == 0.0f)
			{
				const Raylib::Vector2 sprite = texture ? GetSpriteLocalSize(*texture) : Raylib::Vector2{ 0.0f, 0.0f };
				if (local.x == 0.0f) { local.x = sprite.x; }
				if (local.y == 0.0f) { local.y = sprite.y; }
			}

			return {
				std::max(std::abs(local.x * transform.Scale.x), MinColliderSizePixels),
				std::max(std::abs(local.y * transform.Scale.y), MinColliderSizePixels)
			};
		}

		Raylib::Vector2 GetColliderWorldOffset(const BoxColliderComponent& collider,
			const TransformComponent& transform)
		{
			return { collider.Offset.x * transform.Scale.x, collider.Offset.y * transform.Scale.y };
		}
	}//namespace Physics

	static b2BodyType ToBox2DType(RigidbodyType t)
	{
		switch (t) {
		case RigidbodyType::Static:    return b2_staticBody;
		case RigidbodyType::Kinematic: return b2_kinematicBody;
		default:                       return b2_dynamicBody;
		}
	}

	static b2Polygon MakeColliderBox(Raylib::Vector2 sizePixels, Raylib::Vector2 offsetPixels)
	{
		return b2MakeOffsetBox(
			Physics::ToMeters(sizePixels.x * 0.5f),
			Physics::ToMeters(sizePixels.y * 0.5f),
			Physics::ToMeters(offsetPixels),
			b2Rot_identity);
	}

	static bool SameVector(Raylib::Vector2 a, Raylib::Vector2 b)
	{
		return a.x == b.x && a.y == b.y;
	}

	static void* ToUserData(entt::entity entity)
	{
		return reinterpret_cast<void*>(static_cast<uintptr_t>(entt::to_integral(entity)));
	}

	static entt::entity EntityFromShape(b2ShapeId shapeId)
	{
		void* userData = b2Body_GetUserData(b2Shape_GetBody(shapeId));
		return static_cast<entt::entity>(reinterpret_cast<uintptr_t>(userData));
	}

	void PhysicsSystem::OnCreate(World& world)
	{
		m_WorldDef = b2DefaultWorldDef();
		m_WorldDef.gravity = { 0.0f, PhysicsSettings::Gravity };
		m_WorldId = b2CreateWorld(&m_WorldDef);

		auto& registry = world.GetRegistry();
		auto view = registry.view<const TransformComponent, const BoxColliderComponent, const IdentificationComponent>();
		for (auto [entity, transform, collider, identification] : view.each())
		{
			b2BodyDef bodyDef = b2DefaultBodyDef();
			bodyDef.position = Physics::ToMeters(transform.Position);
			bodyDef.rotation = b2MakeRot(transform.Rotation * DEG2RAD);
			bodyDef.userData = ToUserData(entity);

			if (const auto* rb = registry.try_get<RigidbodyComponent>(entity))
			{
				bodyDef.type = ToBox2DType(rb->Type);
				bodyDef.fixedRotation = !rb->CanRotate;
				bodyDef.gravityScale = rb->UseGravity ? 1.0f : 0.0f;
				bodyDef.linearDamping = rb->LinearDamping;
				bodyDef.angularDamping = rb->AngularDamping;
			}
			else
			{
				bodyDef.type = b2_staticBody;
			}

			b2BodyId id = b2CreateBody(m_WorldId, &bodyDef);

			if (!identification.IsActive)
			{
				b2Body_Disable(id);
			}

			b2ShapeDef shapeDef = b2DefaultShapeDef();
			shapeDef.density = collider.Material.Density;
			shapeDef.isSensor = collider.Trigger;
			shapeDef.enableSensorEvents = true;
			shapeDef.enableContactEvents = true;

			// Same size the sprite is drawn at: local size * transform scale.
			const Raylib::Vector2 size = Physics::GetColliderWorldSize(collider, transform,
				registry.try_get<TextureComponent>(entity));
			const Raylib::Vector2 offset = Physics::GetColliderWorldOffset(collider, transform);
			const b2Polygon box = MakeColliderBox(size, offset);

			b2ShapeId shapeId = b2CreatePolygonShape(id, &shapeDef, &box);
			m_EntityBodies[entity] = { id, shapeId, size, offset };
		}
	}

	void PhysicsSystem::OnTick(World& world, float deltaTime)
	{
		SyncEnabledState(world);
		SyncMaterials(world);
		SyncColliderShapes(world);
		ApplyTeleports(world);

		
		while (Time::OnFixedFrameReady())
		{
			ApplyForces(world);
			b2World_Step(m_WorldId, Time::GetFixedDeltaTime(), m_StepCount);
			CollectEvents();
		}

		SyncTransforms(world);

		DispatchEvents(world);
	}

	void PhysicsSystem::SyncEnabledState(World& world)
	{
		auto view = world.GetRegistry().view<const IdentificationComponent, const BoxColliderComponent>();
		for (auto [entity, identification, collider] : view.each())
		{
			auto it = m_EntityBodies.find(entity);
			if (it == m_EntityBodies.end() || !b2Body_IsValid(it->second.Body))
			{
				continue;
			}

			const b2BodyId bodyId = it->second.Body;
			bool active = identification.IsActive;
			bool enabled = b2Body_IsEnabled(bodyId);

			if (active && !enabled)
			{
				b2Body_Enable(bodyId);
			}
			else if (!active && enabled)
			{
				b2Body_Disable(bodyId);
			}
		}
	}

	void PhysicsSystem::SyncMaterials(World& world)
	{
		// Pushes any PhysicsMaterial edits made since last frame into Box2D.
		// Only values that actually changed are sent, so this is cheap when idle.
		auto view = world.GetRegistry().view<const BoxColliderComponent>();
		for (auto [entity, collider] : view.each())
		{
			auto it = m_EntityBodies.find(entity);
			if (it == m_EntityBodies.end() || !b2Shape_IsValid(it->second.Shape))
			{
				continue;
			}

			const b2ShapeId shapeId = it->second.Shape;
			const PhysicsMaterial& material = collider.Material;

			// Box2D asserts on negative density.
			const float density = std::max(0.0f, material.Density);
			if (b2Shape_GetDensity(shapeId) != density)
			{
				// true = recompute the body's mass from its shapes right away.
				b2Shape_SetDensity(shapeId, density, true);
			}

			if (b2Shape_GetFriction(shapeId) != material.Friction)
			{
				b2Shape_SetFriction(shapeId, material.Friction);
			}

			if (b2Shape_GetRestitution(shapeId) != material.Restitution)
			{
				b2Shape_SetRestitution(shapeId, material.Restitution);
			}
		}
	}

	void PhysicsSystem::SyncColliderShapes(World& world)
	{
		
		auto& registry = world.GetRegistry();
		auto view = registry.view<const TransformComponent, const BoxColliderComponent>();
		for (auto [entity, transform, collider] : view.each())
		{
			auto it = m_EntityBodies.find(entity);
			if (it == m_EntityBodies.end() || !b2Shape_IsValid(it->second.Shape))
			{
				continue;
			}

			PhysicsBody& body = it->second;

			const Raylib::Vector2 size = Physics::GetColliderWorldSize(collider, transform,
				registry.try_get<TextureComponent>(entity));
			const Raylib::Vector2 offset = Physics::GetColliderWorldOffset(collider, transform);

			if (SameVector(size, body.Size) && SameVector(offset, body.Offset))
			{
				continue;
			}

			const b2Polygon box = MakeColliderBox(size, offset);
			b2Shape_SetPolygon(body.Shape, &box);

			b2Body_ApplyMassFromShapes(body.Body);

			body.Size = size;
			body.Offset = offset;
		}
	}

	void PhysicsSystem::ApplyForces(World& world)
	{
		auto view = world.GetRegistry().view<const RigidbodyComponent, const AppyForceComponent>();
		for (auto [entity, rb, force] : view.each())
		{
			const bool hasForce = force.Force.x != 0.0f || force.Force.y != 0.0f;
			const bool hasTorque = force.Torque != 0.0f;
			if (!hasForce && !hasTorque)
			{
				continue;
			}

			auto it = m_EntityBodies.find(entity);
			if (it == m_EntityBodies.end())
			{
				continue;
			}

			b2BodyId bodyId = it->second.Body;
			if (!b2Body_IsValid(bodyId) || !b2Body_IsEnabled(bodyId))
			{
				continue;
			}

			// wake = true: a sleeping body silently ignores forces applied with
			// wake = false, which makes a resting player unresponsive.
			if (hasForce)
			{
				b2Body_ApplyForceToCenter(bodyId, { force.Force.x, force.Force.y }, true);
			}

			//Y points down, so Box2D's counter-clockwise positive reads as clockwise on screen.
			if (hasTorque)
			{
				b2Body_ApplyTorque(bodyId, force.Torque, true);
			}
		}
	}

	void PhysicsSystem::CollectEvents()
	{
		// Solid-vs-solid contacts.
		b2ContactEvents contacts = b2World_GetContactEvents(m_WorldId);
		for (int i = 0; i < contacts.beginCount; ++i)
		{
			const b2ContactBeginTouchEvent& ev = contacts.beginEvents[i];
			m_PendingEvents.push_back({ EntityFromShape(ev.shapeIdA), EntityFromShape(ev.shapeIdB),
				CollisionEventType::CollisionEnter });
		}

		for (int i = 0; i < contacts.endCount; ++i)
		{
			const b2ContactEndTouchEvent& ev = contacts.endEvents[i];

			if (!b2Shape_IsValid(ev.shapeIdA) || !b2Shape_IsValid(ev.shapeIdB))
			{
				continue;
			}

			m_PendingEvents.push_back({ EntityFromShape(ev.shapeIdA), EntityFromShape(ev.shapeIdB),
				CollisionEventType::CollisionExit });
		}

		b2SensorEvents sensors = b2World_GetSensorEvents(m_WorldId);
		for (int i = 0; i < sensors.beginCount; ++i)
		{
			const b2SensorBeginTouchEvent& ev = sensors.beginEvents[i];
			m_PendingEvents.push_back({ EntityFromShape(ev.sensorShapeId), EntityFromShape(ev.visitorShapeId),
				CollisionEventType::TriggerEnter });
		}

		for (int i = 0; i < sensors.endCount; ++i)
		{
			const b2SensorEndTouchEvent& ev = sensors.endEvents[i];
			if (!b2Shape_IsValid(ev.sensorShapeId) || !b2Shape_IsValid(ev.visitorShapeId))
			{
				continue;
			}

			m_PendingEvents.push_back({ EntityFromShape(ev.sensorShapeId), EntityFromShape(ev.visitorShapeId),
				CollisionEventType::TriggerExit });
		}
	}

	void PhysicsSystem::SyncTransforms(World& world)
	{
		auto view = world.GetRegistry().view<TransformComponent, RigidbodyComponent>();
		for (auto [entity, transform, rb] : view.each())
		{
			if (rb.Type == RigidbodyType::Static)
			{
				continue;
			}

			auto it = m_EntityBodies.find(entity);
			if (it == m_EntityBodies.end() || !b2Body_IsValid(it->second.Body))
			{
				continue;
			}

			const b2BodyId bodyId = it->second.Body;

			transform.Position = Physics::ToPixels(b2Body_GetPosition(bodyId));
			transform.Rotation = b2Rot_GetAngle(b2Body_GetRotation(bodyId)) * RAD2DEG;

			rb.Velocity = Physics::ToPixels(b2Body_GetLinearVelocity(bodyId));
			rb.AngularVelocity = b2Body_GetAngularVelocity(bodyId) * RAD2DEG;
		}
	}

	void PhysicsSystem::DispatchEvents(World& world)
	{
		auto& registry = world.GetRegistry();

		auto invoke = [&registry](entt::entity self, entt::entity other, CollisionEventType type)
			{
				if (!registry.valid(self) || !registry.valid(other))
				{
					return;
				}

				const auto* callbacks = registry.try_get<CollisionCallbacksComponent>(self);
				if (!callbacks)
				{
					return;
				}

				// Copy the callback: it may remove its own component while running,
				// which would destroy the std::function mid-call.
				CollisionCallback callback;
				switch (type)
				{
				case CollisionEventType::CollisionEnter: callback = callbacks->OnCollisionEnter; break;
				case CollisionEventType::CollisionExit:  callback = callbacks->OnCollisionExit;  break;
				case CollisionEventType::TriggerEnter:   callback = callbacks->OnTriggerEnter;   break;
				case CollisionEventType::TriggerExit:    callback = callbacks->OnTriggerExit;    break;
				}

				if (callback)
				{
					callback(Entity(self, &registry), Entity(other, &registry));
				}
			};

		for (const PendingCollisionEvent& ev : m_PendingEvents)
		{
			invoke(ev.A, ev.B, ev.Type);
			invoke(ev.B, ev.A, ev.Type);
		}

		m_PendingEvents.clear();
	}

	void PhysicsSystem::ApplyTeleports(World& world)
	{
		auto& registry = world.GetRegistry();
		auto view = registry.view<TransformComponent, const TeleportRequestComponent>();

		for (auto [entity, transform, request] : view.each())
		{
			transform.Position = request.Position;

			auto it = m_EntityBodies.find(entity);
			if (it == m_EntityBodies.end() || !b2Body_IsValid(it->second.Body))
			{
				continue;
			}

			const b2BodyId bodyId = it->second.Body;

			b2Body_SetTransform(bodyId, Physics::ToMeters(request.Position), b2Body_GetRotation(bodyId));

			if (request.ResetVelocity)
			{
				b2Body_SetLinearVelocity(bodyId, b2Vec2{ 0.0f, 0.0f });
				b2Body_SetAngularVelocity(bodyId, 0.0f);
			}

			b2Body_SetAwake(bodyId, true);
		}

		registry.clear<TeleportRequestComponent>();
	}

	void PhysicsSystem::OnDestroy(World& world)
	{
		if (b2World_IsValid(m_WorldId))
		{
			b2DestroyWorld(m_WorldId);
		}

		m_EntityBodies.clear();
		m_PendingEvents.clear();
	}

	void PhysicsSystem::OnEnable(World& world)
	{
		//Throw away fixed steps that piled up while disabled.
		while (Time::OnFixedFrameReady()) {}
	}
}//namespace Sengine