#include "Globals.h"
#include "Texture.h"

namespace Sengine
{
	Texture::Texture(const fs::path& path)
	{
		m_RawTexture = Raylib::LoadTexture(path.string().c_str());
	}

	Texture::~Texture()
	{
		if (m_RawTexture.id != 0)
		{
			Raylib::UnloadTexture(m_RawTexture);
		}
	}

	Texture::Texture(Texture&& other) noexcept
		: m_RawTexture(other.m_RawTexture)
	{
		other.m_RawTexture = {}; // steal ownership, leave other empty
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			if (m_RawTexture.id != 0)
			{
				Raylib::UnloadTexture(m_RawTexture);
			}
			m_RawTexture = other.m_RawTexture;
			other.m_RawTexture = {};
		}
		return *this;
	}

	bool Texture::IsValid() const
	{
		return m_RawTexture.id != 0;
	}

	const Raylib::Texture& Texture::GetRawTexture() const
	{
		return m_RawTexture;
	}
}//namespace Sengine