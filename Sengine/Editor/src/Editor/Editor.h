#pragma once

#include <array>

#include "Engine/Assets/AssetManager.h"
#include "Engine/Game/WorldManager.h"

namespace Editor
{
	class Editor : public ILayer
	{
	public:
		Editor();
		~Editor() override = default;

		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;

		void OnCreate() override;
		void OnTick(float deltaTime) override;
		void OnDestroy() override;

	private:
		void SaveWorld();
		void DrawSaveAsPopup();

	private:
		Assets::AssetManager m_AssetManager;
		WorldManager m_WorldManager;

		bool m_bViewportHovered{ false };

		bool m_bOpenSaveAsPopup{ false };
		bool m_bSaveAsFailed{ false };
		std::array<char, 260> m_SaveAsPath{};
	};
}//namespace Editor