#pragma once
#include <memory>
#include <mutex>
#include <SDL_mixer.h>
#include <string_view>
#include <unordered_map>


class AudioManager
{
	friend class SubSystems;


private:
	static std::unordered_map<std::string, std::weak_ptr<Mix_Music>> musicCache;

	static std::unordered_map<std::string, std::weak_ptr<Mix_Chunk>> sfxCache;

	static std::mutex audioMutex;

	static float musicVolume;

	static float sfxVolume;

	static float masterVolume;


	AudioManager();

	~AudioManager();

	AudioManager(const AudioManager&) = delete;

	AudioManager(AudioManager&&) = delete;


	AudioManager& operator=(const AudioManager&) = delete;

	AudioManager& operator=(AudioManager&&) = delete;


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

	static void SetGlobalSFXVolume(int volume /*[0-1]*/); 

	static void SetSFXVolume(float volume /*[0-1]*/, int sfxChannel = -1);

	static void SetMasterVolume(float volume);

	static float GetMasterVolume();

	static float GetMusicGlobalVolume();

	static float GetGlobalSFXVolume();
};