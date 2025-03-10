/**
 *	Filename: Editor.h
 *	Date Created: 21/02/25
 *	Purpose: The main sengine editor functionality
 ***/
#pragma once

namespace SengineEditor
{
	using namespace Sengine;

	struct Player
	{
		std::string Name = "Player";
	};

	class PlayerSerializer : public ISerializable
	{
	public:
		PlayerSerializer(const Player& player)
			: m_Player(player) {}

		virtual ~PlayerSerializer() override = default;

		virtual void SerializeData(nlohmann::json& out) override;
		virtual void DeserializeData(nlohmann::json& in) override;

	private:
		Player m_Player;
	};

	class Editor : public ISengineApp
	{
	public:
		Editor() = default;

		virtual ~Editor() override = default;

		[[nodiscard]] virtual WindowDescription& GetWindowDescription() override;
		[[nodiscard]] virtual bool OnEarlyInit() override;
		[[nodiscard]] virtual bool OnInit() override;
		virtual void OnTick() override;
		virtual void OnDestroy() override;
		virtual void OnLateDestroy() override;

	private:
		WindowDescription m_WindowDescription;
		Player m_Player;
		PlayerSerializer m_PlayerSerializer = {m_Player};
	};
}
