#include "Globals.h"
#include "ComponentSerialization.h"

#include "Engine/Game/Components.h"
#include "Engine/Assets/AssetManager.h"

namespace Sengine
{
	NLOHMANN_JSON_SERIALIZE_ENUM(RigidbodyType, {
		{ RigidbodyType::Static,      "Static" },
		{ RigidbodyType::Kinematic,   "Kinematic" },
		{ RigidbodyType::DynamicBody, "Dynamic" },
		})

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(IdentificationComponent, Name, IsActive, ID)
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(TransformComponent, Position, Scale, Rotation)

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(PhysicsMaterial, Density, Friction, Restitution, RestitutionThreshold)
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(BoxColliderComponent, Size, Offset, Trigger, Material)

		//Velocity and AngularVelocity are written by the physics system (readonly), so only
		//the configuration is saved. Add them here if you want save games to keep momentum.
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(RigidbodyComponent, Type, UseGravity, CanRotate, LinearDamping, AngularDamping)

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(TextComponent, Text, FontColour, FontSize)

		static void SaveTexture(const TextureComponent& c, nlohmann::json& j, const SerializationContext& ctx)
	{
		j["Source"] = c.Source;
		j["Size"] = c.Size;
		j["Tint"] = c.Tint;
		j["Layer"] = c.Layer;

		if (!c.Texture)
		{
			return;
		}

		if (ctx.AssetManager)
		{
			if (std::optional<fs::path> path = ctx.AssetManager->GetTexturePath(*c.Texture))
			{
				j["Texture"] = path->generic_string();
				return;
			}
		}

		Logging::Log(Logging::ELogType::Warning,
			"TextureComponent: texture was not loaded through the AssetManager and won't be saved");
	}

	static void LoadTexture(TextureComponent& c, const nlohmann::json& j, const SerializationContext& ctx)
	{
		const TextureComponent defaults{};
		c.Source = j.value("Source", defaults.Source);
		c.Size = j.value("Size", defaults.Size);
		c.Tint = j.value("Tint", defaults.Tint);
		c.Layer = j.value("Layer", defaults.Layer);

		const std::string path = j.value("Texture", std::string{});
		if (path.empty())
		{
			return;
		}

		if (!ctx.AssetManager)
		{
			Logging::Log(Logging::ELogType::Error,
				"TextureComponent: no AssetManager in context, can't load '" + path + "'");
			return;
		}

		try
		{
			c.Texture = ctx.AssetManager->GetTexture(path);
		}
		catch (const std::exception& e)
		{
			Logging::Log(Logging::ELogType::Error, std::format("TextureComponent: {}", e.what()));
		}
	}

	static void SaveAudio(const AudioComponent& c, nlohmann::json& j, const SerializationContext& ctx)
	{
		j["Volume"] = c.Volume;
		j["Pitch"] = c.Pitch;
		j["PlayOnCreate"] = c.PlayOnCreate;

		if (!c.Clip)
		{
			return;
		}

		if (ctx.AssetManager)
		{
			if (std::optional<fs::path> path = ctx.AssetManager->GetAudioClipPath(*c.Clip))
			{
				j["Clip"] = path->generic_string();
				return;
			}
		}

		Logging::Log(Logging::ELogType::Warning,
			"AudioComponent: clip was not loaded through the AssetManager and won't be saved");
	}

	static void LoadAudio(AudioComponent& c, const nlohmann::json& j, const SerializationContext& ctx)
	{
		const AudioComponent defaults{};
		c.Volume = j.value("Volume", defaults.Volume);
		c.Pitch = j.value("Pitch", defaults.Pitch);
		c.PlayOnCreate = j.value("PlayOnCreate", defaults.PlayOnCreate);

		const std::string path = j.value("Clip", std::string{});
		if (path.empty())
		{
			return;
		}

		if (!ctx.AssetManager)
		{
			Logging::Log(Logging::ELogType::Error,
				"AudioComponent: no AssetManager in context, can't load '" + path + "'");
			return;
		}

		try
		{
			c.Clip = ctx.AssetManager->GetAudioClip(path);
		}
		catch (const std::exception& e)
		{
			Logging::Log(Logging::ELogType::Error, std::format("AudioComponent: {}", e.what()));
		}
	}

	namespace Serialization
	{
		void RegisterComponentSerializers()
		{
			ComponentSerializers::Register<IdentificationComponent>("Identification");
			ComponentSerializers::Register<TransformComponent>("Transform");
			ComponentSerializers::Register<RigidbodyComponent>("Rigidbody");
			ComponentSerializers::Register<BoxColliderComponent>("BoxCollider");
			ComponentSerializers::Register<TextComponent>("Text");
			ComponentSerializers::Register<TextureComponent>("Texture", &SaveTexture, &LoadTexture);
			ComponentSerializers::Register<AudioComponent>("Audio", &SaveAudio, &LoadAudio);
		}
	}//namespace Serialization
}//namespace Sengine