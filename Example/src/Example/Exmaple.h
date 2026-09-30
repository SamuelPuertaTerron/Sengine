namespace Example
{
	class Example : public ILayer
	{
	public:
		Example() = default;
		~Example() override = default;

		void OnCreate() override;
		void OnTick(float deltaTime) override;
		void OnDestroy() override;

	private:
		void CreateFallingObject();
		void CreateGroundObject();
	private:
		World m_World;
		Assets::AssetManager m_Assets;
	};

}//namespace Example