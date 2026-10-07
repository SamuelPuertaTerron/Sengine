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

	std::shared_ptr<AudioClip> AssetManager::GetAudioClip(const fs::path& path)
	{
		return m_AudioClips.Get((GetAudioRoot() / path).lexically_normal());
	}

	std::optional<fs::path> AssetManager::GetAudioClipPath(const AudioClip& clip) const
	{
		std::optional<fs::path> fullPath = m_AudioClips.FindPath(clip);
		if (!fullPath)
		{
			return std::nullopt;
		}
		return fullPath->lexically_relative(GetAudioRoot().lexically_normal());
	}

	fs::path AssetManager::GetAudioRoot() const
	{
		return m_RootPath / "Audio";
	}

	std::shared_ptr<Scripting::Script> AssetManager::GetScript(const fs::path& path)
	{
		return m_LuaScripts.Get((GetScriptsRoot() / path).lexically_normal());
	}

	std::optional<fs::path> AssetManager::GetScriptPath(const Scripting::Script& script) const
	{
		std::optional<fs::path> fullPath = m_LuaScripts.FindPath(script);
		if (!fullPath)
		{
			return std::nullopt;
		}
		return fullPath->lexically_relative(GetScriptsRoot().lexically_normal());
	}

	fs::path AssetManager::GetScriptsRoot() const
	{
		return m_RootPath / "Scripts";
	}
}//namespace Sengine::Assets