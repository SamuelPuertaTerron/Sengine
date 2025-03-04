#include "Globals.h"
#include "JsonSerialization.h"

namespace Sengine
{
	void Serializer::SerializeObject(const fs::path& path, ISerializable& object)
	{
		Json json;

		object.SerializeData(json);

		std::stringstream ss;
		ss << json;

		Filesystem::WriteFile(path, ss.str());
	}

	void Serializer::DeserializeObject(const fs::path& path, ISerializable& object)
	{
		std::ifstream file(path);
		Json json;
		file >> json;

		object.DeserializeData(json);
	}
}