#include "Globals.h"
#include "AssetTraits.h"

namespace Sengine::Assets
{
	std::shared_ptr<Texture> TextureTraits::Load(const fs::path& path)
	{
		return std::make_shared<Texture>(path);
	}

	bool TextureTraits::IsValid(const Texture& texture)
	{
		return texture.IsValid();
	}

	std::shared_ptr<AudioClip> AudioTraits::Load(const fs::path& path)
	{
		return std::make_shared<AudioClip>(path);
	}

	bool AudioTraits::IsValid(const AudioClip& audio)
	{
		return audio.IsValid();
	}
}//namespace Sengine::Assets