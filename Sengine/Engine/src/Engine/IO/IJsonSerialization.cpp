#include "Globals.h"
#include "IJsonSerialization.h"

namespace Sengine
{
	void JsonSerializer::SerializeObject(const fs::path& path, IJsonSerialization& object)
	{
		nlohmann::json json;
		object.SerializeData(json);

		Filesystem::SaveFile(path, json.dump());
	}

	void JsonSerializer::DeserializeObject(const fs::path& path, IJsonSerialization& object)
	{
		std::string fileText = Filesystem::LoadFile(path);
		nlohmann::json json = nlohmann::json::parse(fileText);

		object.DeserializeData(json);
	}
}//namespace Sengine