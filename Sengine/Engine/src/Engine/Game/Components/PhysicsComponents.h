#pragma once

#include <functional>

namespace Sengine
{
	class Entity;

	enum class RigidbodyType : uint8_t
	{
		Static = 0,
		Kinematic,
		DynamicBody
	};

	struct PhysicsMaterial
	{
		float Density{ 1.0f };
		float Friction{ 0.5f };
		float Restitution{ 0.5f };
		float RestitutionThreshold{ 0.5f };
	};

	struct RigidbodyComponent
	{
		RigidbodyType Type{ RigidbodyType::Static };

		bool UseGravity{ true };
		bool CanRotate{ true };

		//Value is readonly, should not set the value. 
		Raylib::Vector2 Velocity{ 0.0f };
		float AngularVelocity{ 0.0f };		// Degrees/s, positive = clockwise on screen. Readonly, like Velocity.
		float LinearDamping{ 0.0f };
		float AngularDamping{ 0.0f };
	};

	struct BoxColliderComponent
	{
		Raylib::Vector2 Size{ 0.0f, 0.0f };
		Raylib::Vector2 Offset{ 0.0f, 0.0f };

		bool Trigger{ false };

		PhysicsMaterial Material{};
	};

	struct AppyForceComponent
	{
		Raylib::Vector2 Force{ 0.0f, 0.0f };
		float Torque{ 0.0f };				// N*m, positive = clockwise on screen.
	};

	struct TeleportRequestComponent
	{
		Raylib::Vector2 Position{ 0.0f, 0.0f };
		bool ResetVelocity{ true };
	};

	using CollisionCallback = std::function<void(Entity self, Entity other)>;

	struct CollisionCallbacksComponent
	{
		CollisionCallback OnCollisionEnter; // two solid colliders started touching
		CollisionCallback OnCollisionExit;  // two solid colliders stopped touching
		CollisionCallback OnTriggerEnter;   // something entered a trigger / this entered a trigger
		CollisionCallback OnTriggerExit;    // something left a trigger / this left a trigger
	};
}//namespace Sengine