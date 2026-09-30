#include "Globals.h"
#include "WorldManager.h"

#include <fstream>
#include <optional>

#include "Engine/Core/Time.h"
#include "Engine/Game/World.h"
#include "Engine/Game/Serialization/WorldSerialization.h"

namespace Sengine
{
	//Reads and parses a world file. Returns nullopt (and logs) if it's missing or malformed.
	static std::optional<nlohmann::json> ReadWorldFile(const fs::path& path)
	{
		try
		{
			return nlohmann::json::parse(Filesystem::LoadFile(path));
		}
		catch (const std::exception& e)
		{
			Logging::Log(Logging::ELogType::Error, std::format(
				"WorldManager: failed to read '{}': {}", path.string(), e.what()));
			return std::nullopt;
		}
	}

	WorldManager::WorldManager(Assets::AssetManager& assetManager)
		: m_AssetManager(assetManager)
	{
	}

	WorldManager::~WorldManager()
	{
		Destroy();
	}

	void WorldManager::Register(const std::string& name, WorldFactory factory, fs::path file)
	{
		m_Worlds[name] = WorldEntry{ std::move(factory), std::move(file) };
	}

	void WorldManager::SetSharedSetup(WorldSetup setup)
	{
		m_SharedSetup = std::move(setup);
	}

	void WorldManager::Request(const std::string& name)
	{
		m_Pending = name;
	}

	void WorldManager::Tick(float deltaTime)
	{
		if (!m_Pending.empty())
		{
			const std::string next = std::move(m_Pending);
			m_Pending.clear();
			SwitchTo(next);
		}

		if (m_Active)
		{
			m_Active->OnTick(deltaTime);
		}
	}

	void WorldManager::Destroy()
	{
		if (m_Active)
		{
			m_Active->OnDestroy();
			m_Active.reset();
			m_ActiveName.clear();
			m_ActiveFile.clear();
		}
	}

	bool WorldManager::SaveActive()
	{
		if (!m_Active)
		{
			Logging::Log(Logging::ELogType::Warning, "WorldManager: no active world to save");
			return false;
		}

		if (m_ActiveFile.empty())
		{
			Logging::Log(Logging::ELogType::Warning,
				"WorldManager: '" + m_ActiveName + "' has no file, use SaveActiveAs");
			return false;
		}

		return WriteWorld(*m_Active, m_ActiveFile);
	}

	bool WorldManager::SaveActiveAs(const fs::path& path)
	{
		if (!m_Active)
		{
			Logging::Log(Logging::ELogType::Warning, "WorldManager: no active world to save");
			return false;
		}

		if (path.empty())
		{
			Logging::Log(Logging::ELogType::Warning, "WorldManager: save path is empty");
			return false;
		}

		if (!WriteWorld(*m_Active, path))
		{
			return false;
		}

		m_ActiveFile = path;
		return true;
	}

	World* WorldManager::GetActive() const
	{
		return m_Active.get();
	}

	const std::string& WorldManager::GetActiveName() const
	{
		return m_ActiveName;
	}

	const fs::path& WorldManager::GetActiveFile() const
	{
		return m_ActiveFile;
	}

	bool WorldManager::HasPendingSwitch() const
	{
		return !m_Pending.empty();
	}

	bool WorldManager::WriteWorld(World& world, const fs::path& path)
	{
		try
		{
			nlohmann::json json;
			WorldSerialization serializer(&world, &m_AssetManager);
			serializer.SerializeData(json);

			std::error_code ec;
			if (path.has_parent_path())
			{
				fs::create_directories(path.parent_path(), ec);
			}

			fs::path tempPath = path;
			tempPath += ".tmp";
			{
				std::ofstream file(tempPath, std::ios::trunc);
				if (!file)
				{
					Logging::Log(Logging::ELogType::Error,
						"WorldManager: could not open '" + tempPath.string() + "' for writing");
					return false;
				}

				file << json.dump(4) << '\n';
				file.close();
				if (file.fail())
				{
					Logging::Log(Logging::ELogType::Error,
						"WorldManager: failed writing '" + tempPath.string() + "'");
					fs::remove(tempPath, ec);
					return false;
				}
			}

			fs::rename(tempPath, path, ec);
			if (ec)
			{
				Logging::Log(Logging::ELogType::Error, std::format(
					"WorldManager: could not replace '{}': {}", path.string(), ec.message()));
				fs::remove(tempPath, ec);
				return false;
			}
		}
		catch (const std::exception& e)
		{
			Logging::Log(Logging::ELogType::Error, std::format(
				"WorldManager: failed to save '{}': {}", path.string(), e.what()));
			return false;
		}

		Logging::Log(Logging::ELogType::Info, "WorldManager: saved '" + path.string() + "'");
		return true;
	}

	void WorldManager::SwitchTo(const std::string& name)
	{
		auto it = m_Worlds.find(name);
		if (it == m_Worlds.end())
		{
			Logging::Log(Logging::ELogType::Error, "WorldManager: no world registered as '" + name + "'");
			return;
		}

		const WorldFactory factory = it->second.Factory;
		const fs::path file = it->second.File;

		std::optional<nlohmann::json> data;
		if (!file.empty() && Filesystem::Exist(file))
		{
			data = ReadWorldFile(file);
			if (!data)
			{
				Logging::Log(Logging::ELogType::Error,
					"WorldManager: not switching to '" + name + "', keeping the current world");
				return;
			}
		}

		Destroy();

		std::unique_ptr<World> world = factory();
		if (!world)
		{
			Logging::Log(Logging::ELogType::Error, "WorldManager: factory for '" + name + "' returned null");
			return;
		}

		if (m_SharedSetup)
		{
			m_SharedSetup(*world);
		}

		if (data)
		{
			WorldSerialization serializer(world.get(), &m_AssetManager);
			serializer.DeserializeData(*data);
		}
		else if (!file.empty())
		{
			Logging::Log(Logging::ELogType::Info,
				"WorldManager: '" + file.string() + "' doesn't exist yet, starting '" + name + "' empty");
		}

		world->OnCreate();

		m_Active = std::move(world);
		m_ActiveName = name;
		m_ActiveFile = file;

		//Loading may have taken a while; drop the fixed steps that piled up
		//so physics doesn't fast-forward on the first frame.
		while (Time::OnFixedFrameReady()) {}

		Logging::Log(Logging::ELogType::Info, "WorldManager: loaded '" + name + "'");
	}
}//namespace Sengine