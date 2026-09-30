#pragma once

#include "Engine/IO/IJsonSerialization.h"
#include "Engine/Game/Serialization/ComponentSerialization.h"

namespace Sengine
{
	class World;

	class WorldSerialization : public IJsonSerialization
	{
	public:
		WorldSerialization(World* world, Assets::AssetManager* assetManager)
			: m_World(world), m_Context{ assetManager } {
		}

		~WorldSerialization() override = default;

		void SerializeData(nlohmann::json& out) override;
		void DeserializeData(const nlohmann::json& in) override;

	private:
		World* m_World;
		SerializationContext m_Context;
	};
}//namespace Sengine