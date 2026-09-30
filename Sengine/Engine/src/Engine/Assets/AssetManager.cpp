#include "Globals.h"
#include "AssetManager.h"

namespace Sengine::Assets
{
	std::shared_ptr<Texture> AssetManager::GetTexture(const fs::path& path)
	{
		return m_Textures.Get((GetTexturesRoot() / path).lexically_normal());
	}

	std::optional<fs::path> AssetManager::GetTexturePath(const Texture& texture) const
	{
		std::optional<fs::path> fullPath = m_Textures.FindPath(texture);
		if (!fullPath)
		{
			return std::nullopt;
		}

		return fullPath->lexically_relative(GetTexturesRoot().lexically_normal());
	}

	fs::path AssetManager::GetTexturesRoot() const
	{
		return m_RootPath / "Textures";
	}
}//namespace Sengine::Assets