#pragma once

#include "nlohmann/json.hpp"

#include "Engine/Core/UUID.h"

namespace nlohmann
{
	template<>
	struct adl_serializer<Raylib::Vector2>
	{
		static void to_json(json& j, const Raylib::Vector2& v)
		{
			j = json::array({ v.x, v.y });
		}

		static void from_json(const json& j, Raylib::Vector2& v)
		{
			v.x = j.at(0).get<float>();
			v.y = j.at(1).get<float>();
		}
	};

	template<>
	struct adl_serializer<Raylib::Rectangle>
	{
		static void to_json(json& j, const Raylib::Rectangle& r)
		{
			j = json::array({ r.x, r.y, r.width, r.height });
		}

		static void from_json(const json& j, Raylib::Rectangle& r)
		{
			r.x = j.at(0).get<float>();
			r.y = j.at(1).get<float>();
			r.width = j.at(2).get<float>();
			r.height = j.at(3).get<float>();
		}
	};

	template<>
	struct adl_serializer<Raylib::Color>
	{
		static void to_json(json& j, const Raylib::Color& c)
		{
			j = json::array({ c.r, c.g, c.b, c.a });
		}

		static void from_json(const json& j, Raylib::Color& c)
		{
			c.r = j.at(0).get<unsigned char>();
			c.g = j.at(1).get<unsigned char>();
			c.b = j.at(2).get<unsigned char>();
			c.a = j.at(3).get<unsigned char>();
		}
	};

	template<>
	struct adl_serializer<Sengine::UUID>
	{
		static void to_json(json& j, const Sengine::UUID& id)
		{
			j = static_cast<uint64_t>(id);
		}

		static void from_json(const json& j, Sengine::UUID& id)
		{
			id = Sengine::UUID(j.get<uint64_t>());
		}
	};
}//namespace nlohmann