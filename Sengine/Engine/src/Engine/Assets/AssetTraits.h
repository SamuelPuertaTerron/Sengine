#pragma once
#include "Engine/Render/Texture.h"
#include "Engine/Game/Audio/AudioClip.h"

namespace Sengine::Assets
{
	struct TextureTraits
	{
		static std::shared_ptr<Texture> Load(const fs::path& path);
		static bool IsValid(const Texture& texture);
	};

	struct AudioTraits
	{
		static std::shared_ptr<AudioClip> Load(const fs::path& path);
		static bool IsValid(const AudioClip& texture);
	};
}//namespace Sengine::Assets