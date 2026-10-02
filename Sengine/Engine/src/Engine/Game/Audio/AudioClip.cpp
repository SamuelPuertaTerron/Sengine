// AudioClip.cpp
#include "Globals.h"
#include "AudioClip.h"

namespace Sengine
{
	AudioClip::AudioClip(const fs::path& path)
	{
		m_RawClip = Raylib::LoadSound(path.string().c_str());
	}

	AudioClip::~AudioClip()
	{
		if (IsValid())
		{
			Raylib::UnloadSound(m_RawClip);
		}
	}

	AudioClip::AudioClip(AudioClip&& other) noexcept
		: m_RawClip(other.m_RawClip)
	{
		other.m_RawClip = {};
	}

	AudioClip& AudioClip::operator=(AudioClip&& other) noexcept
	{
		if (this != &other)
		{
			if (IsValid())
			{
				Raylib::UnloadSound(m_RawClip);
			}
			m_RawClip = other.m_RawClip;
			other.m_RawClip = {};
		}
		return *this;
	}

	void AudioClip::Play() const 
	{ 
		Raylib::PlaySound(m_RawClip); 
	}

	void AudioClip::Stop() const 
	{
		Raylib::StopSound(m_RawClip); 
	}

	void AudioClip::Pause() const 
	{
		Raylib::PauseSound(m_RawClip);
	}

	void AudioClip::Resume() const 
	{
		Raylib::ResumeSound(m_RawClip);
	}

	void AudioClip::SetVolume(float volume) const 
	{
		Raylib::SetSoundVolume(m_RawClip, volume); 
	}

	void AudioClip::SetPitch(float pitch) const
	{
		Raylib::SetSoundPitch(m_RawClip, pitch); 
	}

	bool AudioClip::IsPlaying() const 
	{
		return Raylib::IsSoundPlaying(m_RawClip); 
	}

	bool AudioClip::IsValid() const 
	{
		return Raylib::IsSoundValid(m_RawClip); 
	}
}//namespace Sengine