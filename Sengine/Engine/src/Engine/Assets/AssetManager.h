#pragma once

#include <optional>

#include "Engine/Assets/AssetCache.h"
#include "Engine/Assets/AssetTraits.h"

namespace Sengine::Assets
{
	class AssetManager
	{
	public:
		std::shared_ptr<Texture> GetTexture(const fs::path& path);

		[[nodiscard]] std::optional<fs::path> GetTexturePath(const Texture& texture) const;

	private:
		[[nodiscard]] fs::path GetTexturesRoot() const;

	private:
		fs::path m_RootPath = "Resources/";

		AssetCache<Texture, TextureTraits> m_Textures;
	};
}//namespace Sengine::Assets