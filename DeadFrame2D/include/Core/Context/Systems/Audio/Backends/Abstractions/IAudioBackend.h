#pragma once
#include <string>


struct Mix_Music;
struct Mix_Chunk;


namespace DF2D::Core
{
	class IAudioBackend
	{
	public:
		virtual ~IAudioBackend() = default;


		virtual Mix_Music* LoadMusic(const std::string& path) = 0;

		virtual Mix_Chunk* LoadSFX(const std::string& path) = 0;


		virtual bool PlayMusic(Mix_Music* music, int loops) = 0;

		virtual int PlayChannel(int channel, Mix_Chunk* chunk, int loops) = 0;

		virtual void StopMusic() = 0;

		virtual void StopChannel(int channel) = 0;

		virtual void PauseMusic() = 0;

		virtual void ResumeMusic() = 0;

		virtual void SetMusicVolume(int volume) = 0;

		virtual void SetChannelVolume(int channel, int volume) = 0;

		virtual bool IsMusicPlaying() = 0;
	};
}