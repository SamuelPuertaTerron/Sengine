#include "Globals.h"
#include "JsonSerialization.h"

namespace Sengine
{
	void Serializer::SerializeObject(const fs::path& path, ISerializable& object)
	{
		try
		{
			nlohmann::json json;

			object.SerializeData(json);

			std::stringstream ss;
			ss << json;

			Filesystem::WriteFile(path, ss.str());
		}
		catch (const std::exception& e)
		{
			const std::string errorMessage = e.what();
			Logger::Log("Cannot Serialize Json file " + errorMessage, ELogType::Error, ELogAreaType::Engine);
		}
	}

	void Serializer::DeserializeObject(const fs::path& path, ISerializable& object)
	{
		try
		{
			std::ifstream file(path);
			nlohmann::json json;
			file >> json;

			object.DeserializeData(json);
		}
		catch (const std::exception& e)
		{
			const std::string errorMessage = e.what();
			Logger::Log("Cannot Deserialize Json file " +  errorMessage, ELogType::Error, ELogAreaType::Engine);
		}
	}
}