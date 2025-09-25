#pragma once
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include <memory>
#include <Models/Audio/AudioConfig.h>
#include <mutex>
#include <SDL_mixer.h>
#include <string_view>
#include <unordered_map>


namespace DeadFrame2D::Core
{
	class AudioManager : public ISubSystem
	{
		friend class SubSystems;


	private:
		static AudioManager* instance;


		Shared::Models::AudioConfig audioConfig;

		std::unordered_map<std::string, std::weak_ptr<Mix_Music>> musicCache;

		std::unordered_map<std::string, std::weak_ptr<Mix_Chunk>> sfxCache;

		std::mutex audioMutex;


		AudioManager(const Shared::Models::AudioConfig& audioConfig);

		virtual ~AudioManager() override;

		AudioManager(const AudioManager&) = delete;

		AudioManager(AudioManager&&) = delete;


		AudioManager& operator=(const AudioManager&) = delete;

		AudioManager& operator=(AudioManager&&) = delete;


		virtual void Update(float deltaTime) override;

		virtual void BeginFrame() override;

		virtual void EndUpdate() override;

		virtual void EndDraw() override;


	public:
		static std::shared_ptr<Mix_Music> LoadMusic(const std::string_view& filepath);

		static std::shared_ptr<Mix_Chunk> LoadSFX(const std::string_view& filepath);

		static bool PlayMusicTrack(const std::shared_ptr<Mix_Music>& music, int loopCount = 0);

		static int PlaySFX(const std::shared_ptr<Mix_Chunk>& sfx, int loopCount = 0);

		static void FadeInMusicTrack(const std::shared_ptr<Mix_Music>& music, int loopCount, int fadeTimeMs);

		static void StopMusic();

		static void StopSFX(int sfxChannel);

		static void PauseMusic();

		static void PauseSFX(int sfxChannel);

		static void ResumeMusic();

		static void SetMusicVolume(float volume /*[0-1]*/);

		static void SetGlobalSFXVolume(float volume /*[0-1]*/); 

		static void SetSFXVolume(float volume /*[0-1]*/, int sfxChannel = -1);

		static void SetMasterVolume(float volume);

		static float GetMasterVolume();

		static float GetMusicGlobalVolume();

		static float GetGlobalSFXVolume();
	};
}