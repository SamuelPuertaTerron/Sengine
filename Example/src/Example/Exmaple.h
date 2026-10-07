#pragma once

namespace Example
{
	class Example : public ILayer
	{
	public:
		void OnCreate() override;
		void OnTick(float deltaTime) override;
		void OnDestroy() override;

	private:
		//Builds entities and systems. Used at startup and on every reset.
		void BuildWorld();
		void ResetWorld();

		void CreateRock();
		void CreateGround();
		void CreateHud();

	private:
		World m_World;
		Assets::AssetManager m_Assets;

		bool m_bResetRequested = false;
	};
}//namespace Example