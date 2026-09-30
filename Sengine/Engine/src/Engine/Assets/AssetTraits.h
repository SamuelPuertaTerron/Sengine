#pragma once
#include "Engine/Render/Texture.h"

namespace Sengine::Assets
{
	struct TextureTraits
	{
		static std::shared_ptr<Texture> Load(const fs::path& path);
		static bool IsValid(const Texture& texture);
	};
}//namespace Sengine::Assets