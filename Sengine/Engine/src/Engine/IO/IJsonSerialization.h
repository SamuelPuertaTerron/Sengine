#pragma once

#include "nlohmann/json.hpp"

namespace Sengine
{
	class IJsonSerialization
	{
	public:
		virtual ~IJsonSerialization() = default;

		virtual void SerializeData(nlohmann::json& out) = 0;
		virtual void DeserializeData(const nlohmann::json& in) = 0;
	};

	class JsonSerializer
	{
	public:
		static void SerializeObject(const fs::path& path, IJsonSerialization& object);
		static void DeserializeObject(const fs::path& path, IJsonSerialization& object);
	};
}//namespace Sengine
