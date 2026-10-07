#pragma once
#include "Engine/Render/Texture.h"
#include "Engine/Game/Audio/AudioClip.h"
#include "Engine/Game/Scripting/Script.h"

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
		static bool IsValid(const AudioClip& audio);
	};

	struct LuaTraits
	{
		static std::shared_ptr<Scripting::Script> Load(const fs::path& path);
		static bool IsValid(const Scripting::Script& script);
	};
}//namespace Sengine::Assets