#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "entt/entt.hpp"

#include "Engine/Game/Serialization/JsonSerializationTypes.h"

namespace Sengine
{
	namespace Assets { class AssetManager; }

	struct SerializationContext
	{
		Assets::AssetManager* AssetManager = nullptr;
	};

	struct ComponentSerializer
	{
		std::function<bool(const entt::registry&, entt::entity)> Has;
		std::function<void(const entt::registry&, entt::entity, nlohmann::json&, const SerializationContext&)> Save;
		std::function<void(entt::registry&, entt::entity, const nlohmann::json&, const SerializationContext&)> Load;
	};

	template<typename T>
	using ComponentSaveFn = void(*)(const T&, nlohmann::json&, const SerializationContext&);

	template<typename T>
	using ComponentLoadFn = void(*)(T&, const nlohmann::json&, const SerializationContext&);

	class ComponentSerializers
	{
	public:
		template<typename T>
		static void Register(const std::string& name)
		{
			Get()[name] = ComponentSerializer{
				[](const entt::registry& r, entt::entity e) { return r.all_of<T>(e); },
				[](const entt::registry& r, entt::entity e, nlohmann::json& j, const SerializationContext&) { j = r.get<T>(e); },
				[](entt::registry& r, entt::entity e, const nlohmann::json& j, const SerializationContext&) { r.emplace_or_replace<T>(e, j.get<T>()); }
			};
		}

		template<typename T>
		static void Register(const std::string& name, ComponentSaveFn<T> save, ComponentLoadFn<T> load)
		{
			Get()[name] = ComponentSerializer{
				[](const entt::registry& r, entt::entity e) { return r.all_of<T>(e); },
				[save](const entt::registry& r, entt::entity e, nlohmann::json& j, const SerializationContext& ctx)
				{
					save(r.get<T>(e), j, ctx);
				},
				[load](entt::registry& r, entt::entity e, const nlohmann::json& j, const SerializationContext& ctx)
				{
					T component{};
					load(component, j, ctx);
					r.emplace_or_replace<T>(e, std::move(component));
				}
			};
		}

		static std::unordered_map<std::string, ComponentSerializer>& Get()
		{
			static std::unordered_map<std::string, ComponentSerializer> s_Serializers;
			return s_Serializers;
		}
	};

	namespace Serialization
	{
		void RegisterComponentSerializers();
	}//namespace Serialization
}//namespace Sengine