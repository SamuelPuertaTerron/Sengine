#pragma once

namespace Sengine
{
	class Texture;

	struct TextureComponent
	{
		std::shared_ptr<Texture> Texture;
		Raylib::Rectangle Source{};
		Raylib::Vector2 Size{ 0.0f, 0.0f };
		Raylib::Color Tint = Raylib::Color(255, 255, 255, 255);
		int16_t Layer{ 0 };
	};

	Raylib::Vector2 GetSpriteLocalSize(const TextureComponent& sprite);
}//namespace Sengine