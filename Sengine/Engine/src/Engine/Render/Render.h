#pragma once

namespace Sengine
{
	class Texture;

	enum class ScaleMode
	{
		Integer,	//Whole-number scaling only (1x, 2x, 3x...). Every game pixel is the same size on screen. Best for pixel art.
		Fit			//Any scale that fills the window. Bigger picture, but some pixels can end up 1px wider than others.
	};

	struct RenderSettings
	{
		int VirtualWidth = 640;									
		int VirtualHeight = 360;								
		ScaleMode Scaling = ScaleMode::Integer;
		Raylib::Color LetterboxColor = Raylib::Color(0, 0, 0, 255);	
	};

	struct SpriteCommand
	{
		const Texture* Texture = nullptr;					
		Raylib::Rectangle Source{};							
		Raylib::Rectangle Dest{};							
		Raylib::Vector2 Origin{};							
		float Rotation = 0.0f;								
		Raylib::Color Tint = Raylib::Color(255, 255, 255, 255);
		int16_t Layer = 0;									
	};

	namespace Renderer2D
	{
		void Init(const RenderSettings& settings, std::size_t reserveCommandCount = 4096);
		void Destroy();

		void BeginFrame(Raylib::Color clearColor);
		void Present();
		void EndFrame();

		void BeginScene(const Raylib::Camera2D& camera);
		void Submit(const SpriteCommand& command);
		void EndScene();

		void SetHostRect(Raylib::Rectangle windowRect);
		void ClearHostRect();
		[[nodiscard]] const Raylib::Texture& GetCanvasTexture();	

		[[nodiscard]] Raylib::Vector2 GetVirtualSize();
		[[nodiscard]] Raylib::Rectangle GetViewport();							
		[[nodiscard]] float GetScale();											
		[[nodiscard]] Raylib::Vector2 WindowToVirtual(Raylib::Vector2 windowPosition);
		[[nodiscard]] Raylib::Rectangle GetVisibleWorldRect(const Raylib::Camera2D& camera);

		[[nodiscard]] std::size_t GetLastSceneCommandCount();
	}//namespace Renderer2D
}//namespace Sengine