#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Audio/Abstractions/IAudioBackend.h"
#include "Data/Systems/Audio/AudioResourceID.h"
#include "DF2D_API.h"
#include "Models/Audio/AudioConfig.h"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>


namespace DF2D::Core
{
	class DF2D_API AudioManager : public ICoreSystem
	{
		friend class SystemInitializer;


	private:
		Models::AudioConfig audioConfig;

		std::unique_ptr<IAudioBackend> backend;

		std::unordered_map<std::string, Data::AudioResourceID> musicCache;

		std::unordered_map<std::string, Data::AudioResourceID> sfxCache;

		std::mutex mutex;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		AudioManager(Models::AudioConfig audioConfig, std::unique_ptr<IAudioBackend> backend);


		Data::AudioResourceID LoadMusic(const std::string& filePath);

		Data::AudioResourceID LoadSFX(const std::string& filePath);


		bool PlayMusics(Data::AudioResourceID music, int loops = 0);

		int PlaySFX(Data::AudioResourceID sfx, int loops = 0);

		void StopMusic();

		void StopChannel(int c);

		void PauseMusic();

		void ResumeMusic();

		void PauseChannel(int channel);

		void ResumeChannel(int channel);


		void SetMasterVolume(float v);

		void SetMusicVolume(float volume);

		void SetSFXVolume(float volume, int sfxChannel);

		float GetMasterVolume() const;

		float GetMusicVolume() const;

		float GetSFXVolume() const;
	};
}