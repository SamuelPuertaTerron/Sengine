#include "Globals.h"
#include "RenderComponents.h"

#include "Engine/Render/Texture.h"

namespace Sengine
{
	Raylib::Vector2 GetSpriteLocalSize(const TextureComponent& sprite)
	{
		Raylib::Vector2 size = sprite.Size;

		if (size.x == 0.0f) { size.x = std::abs(sprite.Source.width); }
		if (size.y == 0.0f) { size.y = std::abs(sprite.Source.height); }

		if (sprite.Texture && sprite.Texture->IsValid())
		{
			const Raylib::Texture& raw = sprite.Texture->GetRawTexture();
			if (size.x == 0.0f) { size.x = static_cast<float>(raw.width); }
			if (size.y == 0.0f) { size.y = static_cast<float>(raw.height); }
		}

		return size;
	}
}//namespace Sengine
