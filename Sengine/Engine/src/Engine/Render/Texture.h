#pragma once

namespace Sengine
{
	class Texture
	{
	public:
		explicit Texture(const fs::path& path);
		~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;

		bool IsValid() const;
		const Raylib::Texture& GetRawTexture() const;

	private:
		Raylib::Texture m_RawTexture{};
	};
}//namespace Sengine