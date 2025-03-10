/**
 *	Filename: JsonSerialization.h
 *	Date Created: 04/03/25
 *	Purpose: Wrapper around nlohmann Json library
 ***/
#pragma once

#include "nlohmann/json.hpp"

namespace Sengine
{
	class ISerializable
	{
	public:
		virtual ~ISerializable() = default;

		virtual void SerializeData(nlohmann::json& out) = 0;
		virtual void DeserializeData(nlohmann::json& in) = 0;
	};

	class Serializer
	{
	public:
		static void SerializeObject(const fs::path& path, ISerializable& object);
		static void DeserializeObject(const fs::path& path, ISerializable& object);
	};
}