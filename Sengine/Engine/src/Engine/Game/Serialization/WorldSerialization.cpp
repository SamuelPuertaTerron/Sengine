#include "Globals.h"
#include "WorldSerialization.h"

#include "Engine/Game/World.h"
#include "Engine/Game/Components.h"

namespace Sengine
{
	static constexpr int k_WorldFormatVersion = 1;

	void WorldSerialization::SerializeData(nlohmann::json& out)
	{
		entt::registry& reg = m_World->GetRegistry();
		const auto& serializers = ComponentSerializers::Get();

		out["version"] = k_WorldFormatVersion;
		nlohmann::json& entities = out["entities"] = nlohmann::json::array();

		for (entt::entity handle : reg.view<IdentificationComponent>())
		{
			nlohmann::json components = nlohmann::json::object();
			for (const auto& [name, serializer] : serializers)
			{
				if (serializer.Has(reg, handle))
				{
					serializer.Save(reg, handle, components[name], m_Context);
				}
			}

			nlohmann::json entity;
			entity["components"] = std::move(components);
			entities.push_back(std::move(entity));
		}
	}

	void WorldSerialization::DeserializeData(const nlohmann::json& in)
	{
		if (!in.is_object())
		{
			Logging::Log(Logging::ELogType::Error, "WorldSerialization: root is not a JSON object");
			return;
		}

		const int version = in.value("version", 0);
		if (version != k_WorldFormatVersion)
		{
			Logging::Log(Logging::ELogType::Warning, std::format(
				"WorldSerialization: file version is {}, expected {}", version, k_WorldFormatVersion));
		}

		const auto entitiesIt = in.find("entities");
		if (entitiesIt == in.end() || !entitiesIt->is_array())
		{
			Logging::Log(Logging::ELogType::Error, "WorldSerialization: missing 'entities' array");
			return;
		}

		entt::registry& reg = m_World->GetRegistry();
		const auto& serializers = ComponentSerializers::Get();

		for (const nlohmann::json& entityJson : *entitiesIt)
		{
			const entt::entity handle = reg.create();

			const auto componentsIt = entityJson.find("components");
			if (componentsIt != entityJson.end() && componentsIt->is_object())
			{
				for (const auto& item : componentsIt->items())
				{
					const std::string& name = item.key();

					const auto serializerIt = serializers.find(name);
					if (serializerIt == serializers.end())
					{
						Logging::Log(Logging::ELogType::Warning, "WorldSerialization: unknown component '" + name + "'");
						continue;
					}

					//One malformed component shouldn't abort the whole world.
					try
					{
						serializerIt->second.Load(reg, handle, item.value(), m_Context);
					}
					catch (const std::exception& e)
					{
						Logging::Log(Logging::ELogType::Error, std::format(
							"WorldSerialization: failed to load component '{}': {}", name, e.what()));
					}
				}
			}

			//Every entity must have an identity, same as World::CreateEntity guarantees.
			//Without it the entity would also be skipped on the next save.
			if (!reg.all_of<IdentificationComponent>(handle))
			{
				reg.emplace<IdentificationComponent>(handle);
			}
		}
	}
}//namespace Sengine