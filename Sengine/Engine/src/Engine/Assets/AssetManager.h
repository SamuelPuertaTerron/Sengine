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

		std::shared_ptr<AudioClip> GetAudioClip(const fs::path& path);
		[[nodiscard]] std::optional<fs::path> GetAudioClipPath(const AudioClip& clip) const;

		std::shared_ptr<Scripting::Script> GetScript(const fs::path& path);
		[[nodiscard]] std::optional<fs::path> GetScriptPath(const Scripting::Script& script) const;

	private:
		[[nodiscard]] fs::path GetTexturesRoot() const;
		[[nodiscard]] fs::path GetAudioRoot() const;
		[[nodiscard]] fs::path GetScriptsRoot() const;

	private:
		fs::path m_RootPath = "Resources/";

		AssetCache<Texture, TextureTraits> m_Textures;
		AssetCache<AudioClip, AudioTraits> m_AudioClips;
		AssetCache<Scripting::Script, LuaTraits> m_LuaScripts;
	};
}//namespace Sengine::Assets