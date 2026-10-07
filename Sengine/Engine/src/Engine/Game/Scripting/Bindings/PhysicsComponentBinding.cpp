#include "Globals.h"
#include "ScriptBindingsDetail.h"

#include "Engine/Game/Components.h"

namespace Sengine::Scripting::Detail
{
	void RegisterPhysicsComponents(sol::state& lua, BindingContext& context)
	{
		//The PhysicsSystem only reads rigidbody settings when the world is created,
		//and only creates bodies then, so these are read-only and can't be added/removed.
		BindComponent<RigidbodyComponent>(lua, context, "RigidbodyComponent", false,
			"Velocity", sol::readonly(&RigidbodyComponent::Velocity),
			"AngularVelocity", sol::readonly(&RigidbodyComponent::AngularVelocity),
			"UseGravity", sol::readonly(&RigidbodyComponent::UseGravity),
			"CanRotate", sol::readonly(&RigidbodyComponent::CanRotate),
			"LinearDamping", sol::readonly(&RigidbodyComponent::LinearDamping),
			"AngularDamping", sol::readonly(&RigidbodyComponent::AngularDamping));

		//Size, Offset and Material are synced to Box2D every frame; Trigger is not.
		BindComponent<BoxColliderComponent>(lua, context, "BoxColliderComponent", false,
			"Size", &BoxColliderComponent::Size,
			"Offset", &BoxColliderComponent::Offset,
			"Trigger", sol::readonly(&BoxColliderComponent::Trigger),
			"Material", &BoxColliderComponent::Material);

		BindComponent<AppyForceComponent>(lua, context, "ApplyForceComponent", true,
			"Force", &AppyForceComponent::Force,
			"Torque", &AppyForceComponent::Torque);

		BindComponent<TeleportRequestComponent>(lua, context, "TeleportRequestComponent", true,
			"Position", &TeleportRequestComponent::Position,
			"ResetVelocity", &TeleportRequestComponent::ResetVelocity);
	}
}//namespace Sengine::Scripting::Detail