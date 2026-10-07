#include "Globals.h"
#include "ScriptBindingsDetail.h"

#include "Engine/Game/Components.h"

namespace Sengine::Scripting::Detail
{
	void RegisterBaseComponents(sol::state& lua, BindingContext& context)
	{
		BindComponent<IdentificationComponent>(lua, context, "IdentificationComponent", false,
			"Name", &IdentificationComponent::Name,
			"IsActive", &IdentificationComponent::IsActive);

		BindComponent<TransformComponent>(lua, context, "TransformComponent", true,
			"Position", &TransformComponent::Position,
			"Scale", &TransformComponent::Scale,
			"Rotation", &TransformComponent::Rotation);

		BindComponent<TextureComponent>(lua, context, "TextureComponent", true,
			"Size", &TextureComponent::Size,
			"Tint", &TextureComponent::Tint,
			"Layer", &TextureComponent::Layer);

		BindComponent<TextComponent>(lua, context, "TextComponent", false,
			"Text", &TextComponent::Text);

		BindComponent<PlaySoundRequestComponent>(lua, context, "PlaySoundRequestComponent", true);
	}
}//namespace Sengine::Scripting::Detail