#pragma once

namespace Sengine
{
	class World;
	namespace Assets { class AssetManager; }

	using WorldFactory = std::function<std::unique_ptr<World>()>;
	using WorldSetup = std::function<void(World&)>;

	class WorldManager
	{
	public:
		explicit WorldManager(Assets::AssetManager& assetManager);
		~WorldManager();	//Defined in the .cpp, where World is a complete type.

		WorldManager(const WorldManager&) = delete;
		WorldManager& operator=(const WorldManager&) = delete;

		void Register(const std::string& name, WorldFactory factory, fs::path file = {});
		void SetSharedSetup(WorldSetup setup);

		void Request(const std::string& name);

		void Tick(float deltaTime);
		void Destroy();

		bool SaveActive();

		bool SaveActiveAs(const fs::path& path);

		[[nodiscard]] World* GetActive() const;
		[[nodiscard]] const std::string& GetActiveName() const;
		[[nodiscard]] const fs::path& GetActiveFile() const;
		[[nodiscard]] bool HasPendingSwitch() const;

	private:
		struct WorldEntry
		{
			WorldFactory Factory;
			fs::path File;
		};

		void SwitchTo(const std::string& name);
		bool WriteWorld(World& world, const fs::path& path);

	private:
		Assets::AssetManager& m_AssetManager;

		std::unique_ptr<World> m_Active;
		std::unordered_map<std::string, WorldEntry> m_Worlds;
		WorldSetup m_SharedSetup;

		std::string m_ActiveName;
		fs::path m_ActiveFile;
		std::string m_Pending;
	};
}//namespace Sengine