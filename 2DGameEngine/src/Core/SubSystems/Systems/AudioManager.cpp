#include "Core/SubSystems/Systems/AudioManager.h"
#include <algorithm>
#include <cassert>
#include <iostream>


namespace DeadFrame2D::Core
{
	using namespace Shared::Models;


	AudioManager* AudioManager::instance;


	AudioManager::AudioManager(const AudioConfig& audioConfig)
		: audioConfig(audioConfig)
	{
		assert(instance == nullptr && "AudioManager was already initialized!");

		musicCache.clear();
		sfxCache.clear();

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;

			return;
		}

		Mix_AllocateChannels(audioConfig.maxSFXChannelAllocation);

		std::cout << "[Info] SDL_Mixer successfully initialized." << std::endl;
	}

	AudioManager::~AudioManager()
	{
		musicCache.clear();
		sfxCache.clear();

		Mix_CloseAudio();

		std::cout << "[Info] SDL_Mixer subsystem successfully quit." << std::endl;
	}

	void AudioManager::Update(float deltaTime)
	{

	}

	void AudioManager::BeginFrame()
	{

	}

	void AudioManager::EndUpdate()
	{

	}

	void AudioManager::EndDraw()
	{

	}

	std::shared_ptr<Mix_Music> AudioManager::LoadMusic(const std::string_view& filepath)
	{
		auto filePathString = std::string(filepath);

		std::lock_guard<std::mutex> lock(instance->audioMutex);

		auto it = instance->musicCache.find(filePathString);

		if (it != instance->musicCache.end())
		{
			if (auto cached = it->second.lock())
				return cached;
		}

		auto raw = Mix_LoadMUS(filePathString.c_str());

		if (raw == nullptr)
		{
			std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
		
			return nullptr;
		}

		auto shared = std::shared_ptr<Mix_Music>(raw, Mix_FreeMusic);

		instance->musicCache[filePathString] = shared;
	
		return shared;
	}

	std::shared_ptr<Mix_Chunk> AudioManager::LoadSFX(const std::string_view& filepath)
	{
		auto filePathString = std::string(filepath);

		std::lock_guard<std::mutex> lock(instance->audioMutex);

		auto it = instance->sfxCache.find(filePathString);

		if (it != instance->sfxCache.end())
		{
			if (auto cached = it->second.lock())
				return cached;
		}

		auto raw = Mix_LoadWAV(filePathString.c_str());

		if (raw == nullptr)
		{
			std::cerr << "Failed to load SFX: " << Mix_GetError() << std::endl;
		
			return nullptr;
		}

		auto shared = std::shared_ptr<Mix_Chunk>(raw, Mix_FreeChunk);

		instance->sfxCache[filePathString] = shared;

		return shared;
	}

	bool AudioManager::PlayMusicTrack(const std::shared_ptr<Mix_Music>& music, int loopCount)
	{
		if (music == nullptr)
			return false;

		auto musicVolume = instance->audioConfig.musicVolume;
		auto masterVolume = instance->audioConfig.masterVolume;

		Mix_VolumeMusic(static_cast<int>(musicVolume * masterVolume * MIX_MAX_VOLUME));

		return Mix_PlayMusic(music.get(), loopCount) == 0;
	}

	int AudioManager::PlaySFX(const std::shared_ptr<Mix_Chunk>& sfx, int loopCount)
	{
		if (sfx == nullptr)
			return -1;

		auto channel = -1;

		// Play on the first free channel (-1)
		channel = Mix_PlayChannel(-1, sfx.get(), loopCount);

		if (channel == -1)
		{
			std::cerr << "Failed to play SFX: " << Mix_GetError() << std::endl;

			return -1;
		}

		auto sfxVolume = instance->audioConfig.sfxVolume;
		auto masterVolume = instance->audioConfig.masterVolume;

		Mix_Volume(channel, static_cast<int>(sfxVolume * masterVolume * MIX_MAX_VOLUME));

		// Set the chunk volume to max by default (let the volume be controlled by the channel)
		Mix_VolumeChunk(sfx.get(), MIX_MAX_VOLUME);

		return channel;
	}

	void AudioManager::FadeInMusicTrack(const std::shared_ptr<Mix_Music>& music, int loopCount, int fadeTimeMs)
	{
		if (Mix_PlayingMusic())
		{
			Mix_FadeOutMusic(fadeTimeMs);
		}

		auto musicVolume = instance->audioConfig.musicVolume;
		auto masterVolume = instance->audioConfig.masterVolume;

		Mix_VolumeMusic(static_cast<int>(musicVolume * masterVolume * MIX_MAX_VOLUME));

		Mix_FadeInMusic(music.get(), loopCount, fadeTimeMs);
	}

	void AudioManager::StopMusic()
	{
		Mix_HaltMusic();
	}

	void AudioManager::StopSFX(int sfxChannel)
	{
		Mix_HaltChannel(sfxChannel);
	}

	void AudioManager::PauseMusic()
	{
		Mix_PauseMusic();
	}

	void AudioManager::PauseSFX(int sfxChannel)
	{
		Mix_Pause(sfxChannel);
	}

	void AudioManager::ResumeMusic()
	{
		Mix_ResumeMusic();
	}

	void AudioManager::SetMusicVolume(float volume)
	{
		auto newMusicVolume = instance->audioConfig.musicVolume = std::clamp(volume, 0.0f, 1.0f);
		auto masterVolume = instance->audioConfig.masterVolume;

		Mix_VolumeMusic(static_cast<int>(newMusicVolume * masterVolume * MIX_MAX_VOLUME));
	}

	void AudioManager::SetGlobalSFXVolume(float volume)
	{
		auto newSfxVolume = instance->audioConfig.sfxVolume = std::clamp(volume, 0.0f, 1.0f);

		SetSFXVolume(newSfxVolume);
	}

	void AudioManager::SetSFXVolume(float volume, int sfxChannel)
	{
		volume = std::clamp(volume, 0.0f, 1.0f);

		auto sfxVolume = instance->audioConfig.sfxVolume;
		auto masterVolume = instance->audioConfig.masterVolume;

		// If sfxChannel == -1, it will set the volume of all channels
		Mix_Volume(sfxChannel, static_cast<int>(volume * sfxVolume * masterVolume * MIX_MAX_VOLUME));
	}

	void AudioManager::SetMasterVolume(float volume)
	{
		auto newMasterVolume = instance->audioConfig.masterVolume = std::clamp(volume, 0.0f, 1.0f);
		auto sfxVolume = instance->audioConfig.sfxVolume;

		SetMusicVolume(newMasterVolume);
		SetSFXVolume(sfxVolume);
	}

	float AudioManager::GetMasterVolume()
	{
		return instance->audioConfig.masterVolume;
	}

	float AudioManager::GetMusicGlobalVolume()
	{
		return instance->audioConfig.musicVolume;
	}

	float AudioManager::GetGlobalSFXVolume()
	{
		return instance->audioConfig.sfxVolume;
	}
}