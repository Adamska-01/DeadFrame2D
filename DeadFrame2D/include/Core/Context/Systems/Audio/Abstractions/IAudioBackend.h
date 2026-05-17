#pragma once
#include "DF2D_API.h"
#include "Data/Systems/Audio/AudioResourceID.h"
#include <cstdint>
#include <string>


namespace DF2D::Core
{
	class DF2D_API IAudioBackend
	{
	public:
		virtual ~IAudioBackend() = default;


		virtual Data::AudioResourceID LoadMusic(const std::string& path) = 0;

		virtual Data::AudioResourceID LoadSFX(const std::string& path) = 0;


		virtual bool PlayMusic(Data::AudioResourceID music, int loops) = 0;

		virtual int PlayChannel(int channel, Data::AudioResourceID sfx, int loops) = 0;

		virtual void StopMusic() = 0;

		virtual void StopChannel(int channel) = 0;

		virtual void PauseMusic() = 0;

		virtual void ResumeMusic() = 0;

		virtual void PauseChannel(int channel) = 0;

		virtual void ResumeChannel(int channel) = 0;

		virtual void SetMusicVolume(float volume) = 0;

		virtual void SetChannelVolume(int channel, float volume) = 0;

		virtual bool IsMusicPlaying() = 0;
	};
}