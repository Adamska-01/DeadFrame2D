#pragma once
#include "Core/Context/Systems/Audio/Backends/Abstractions/IAudioBackend.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	class DF2D_API SDLAudioBackend final : public IAudioBackend
	{
	public:
		SDLAudioBackend(int maxSFXChannelAllocation);
		
		~SDLAudioBackend() override;


		Mix_Music* LoadMusic(const std::string& path) override;

		Mix_Chunk* LoadSFX(const std::string& path) override;


		bool PlayMusic(Mix_Music* music, int loops) override;

		int PlayChannel(int channel, Mix_Chunk* chunk, int loops) override;

		void StopMusic() override;

		void StopChannel(int c) override;

		void PauseMusic() override;

		void ResumeMusic() override;

		void SetMusicVolume(int volume) override;

		void SetChannelVolume(int channel, int volume) override;

		bool IsMusicPlaying() override;
	};
}