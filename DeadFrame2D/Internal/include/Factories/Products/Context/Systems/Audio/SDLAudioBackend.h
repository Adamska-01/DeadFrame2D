#pragma once
#include "Core/Context/Systems/Audio/Abstractions/IAudioBackend.h"
#include "Models/Audio/AudioConfig.h"
#include <memory>
#include <SDL_mixer.h>
#include <unordered_map>


namespace DF2D::Internal
{
	class DF2D_API SDLAudioBackend final : public Core::IAudioBackend
	{
	private:
		std::unordered_map<Data::AudioResourceID, std::shared_ptr<Mix_Music>> musicResources;

		std::unordered_map<Data::AudioResourceID, std::shared_ptr<Mix_Chunk>> sfxResources;

		Data::AudioResourceID nextID;


	public:
		SDLAudioBackend(Models::AudioConfig config);

		~SDLAudioBackend() override;


		Data::AudioResourceID LoadMusic(const std::string& path) override;

		Data::AudioResourceID LoadSFX(const std::string& path) override;


		bool PlayMusic(Data::AudioResourceID music, int loops) override;

		int PlayChannel(int channel, Data::AudioResourceID sfx, int loops) override;

		void StopMusic() override;

		void StopChannel(int c) override;

		void PauseMusic() override;

		void ResumeMusic() override;

		void PauseChannel(int channel) override;

		void ResumeChannel(int channel) override;

		void SetMusicVolume(float volume) override;

		void SetChannelVolume(int channel, float volume) override;

		bool IsMusicPlaying() override;

		bool IsChannelPlaying(int channel) override;
	};
}