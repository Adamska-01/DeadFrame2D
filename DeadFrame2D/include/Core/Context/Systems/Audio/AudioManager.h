#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Audio/Backends/Abstractions/IAudioBackend.h"
#include "DF2D_API.h"
#include "Models/Audio/AudioConfig.h"
#include <memory>
#include <mutex>
#include <string>
#include <SDL_mixer.h>
#include <unordered_map>


namespace DF2D::Core
{
	class DF2D_API AudioManager
	{
	private:
		Models::AudioConfig audioConfig;

		std::unique_ptr<IAudioBackend> backend;

		std::unordered_map<std::string, std::weak_ptr<Mix_Music>> musicCache;

		std::unordered_map<std::string, std::weak_ptr<Mix_Chunk>> sfxCache;

		std::mutex mutex;


	public:
		AudioManager(Models::AudioConfig audioConfig, std::unique_ptr<IAudioBackend> backend);


		std::shared_ptr<Mix_Music> LoadMusic(const std::string& filePath);

		std::shared_ptr<Mix_Chunk> LoadSFX(const std::string& filePath);


		bool PlayMusics(const std::shared_ptr<Mix_Music>& music, int loops = 0);

		int PlaySFX(const std::shared_ptr<Mix_Chunk>& sfx, int loops = 0);

		void StopMusic();

		void StopChannel(int c);

		void PauseMusic();

		void ResumeMusic();


		void SetMasterVolume(float v);

		void SetMusicVolume(float volume);

		void SetSFXVolume(float volume, int sfxChannel);

		float GetMasterVolume() const;

		float GetMusicVolume() const;

		float GetSFXVolume() const;
	};
}