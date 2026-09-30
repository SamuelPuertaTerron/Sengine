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
		//Scene setup, called once from OnCreate.
		void CreateRock();
		void CreateGround();
		void CreateHud();

		void OnRockCollision(Entity self, Entity other);

	private:
		World m_World;
		Assets::AssetManager m_Assets;

		Entity m_BounceText;
		int m_Bounces = 0;
	};
}//namespace Example