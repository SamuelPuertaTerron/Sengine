// AudioClip.h
#pragma once

namespace Sengine
{
	class AudioClip
	{
	public:
		explicit AudioClip(const fs::path& path);
		~AudioClip();

		AudioClip(const AudioClip&) = delete;
		AudioClip& operator=(const AudioClip&) = delete;
		AudioClip(AudioClip&& other) noexcept;
		AudioClip& operator=(AudioClip&& other) noexcept;

		void Play() const;
		void Stop() const;
		void Pause() const;
		void Resume() const;

		void SetVolume(float volume) const;	//0..1
		void SetPitch(float pitch) const;	//1 = normal

		[[nodiscard]] bool IsPlaying() const;
		[[nodiscard]] bool IsValid() const;

	private:
		Raylib::Sound m_RawClip{};
	};
}//namespace Sengine