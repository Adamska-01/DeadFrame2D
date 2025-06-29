#include <algorithm>
#include <Constants/AudioConstants.h>
#include <iostream>
#include <SubSystems/AudioManager.h>


std::unordered_map<std::string, std::weak_ptr<Mix_Music>> AudioManager::musicCache;

std::unordered_map<std::string, std::weak_ptr<Mix_Chunk>> AudioManager::sfxCache;

std::mutex AudioManager::audioMutex;

float AudioManager::musicVolume = 1.0f;

float AudioManager::sfxVolume = 1.0f;

float AudioManager::masterVolume = 1.0f;


AudioManager::AudioManager()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;

		return;
	}

	Mix_AllocateChannels(AudioConstants::MAX_SFX_CHANNEL_ALLOCATION);

	std::cout << "[Info] SDL_Mixer successfully initialized." << std::endl;
}

AudioManager::~AudioManager()
{
	musicCache.clear();
	sfxCache.clear();

	Mix_CloseAudio();

	std::cout << "[Info] SDL_Mixer subsystem successfully quit." << std::endl;
}

std::shared_ptr<Mix_Music> AudioManager::LoadMusic(const std::string_view& filepath)
{
	auto filePathString = std::string(filepath);

	std::lock_guard<std::mutex> lock(audioMutex);

	auto it = musicCache.find(filePathString);

	if (it != musicCache.end())
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

	musicCache[filePathString] = shared;
	
	return shared;
}

std::shared_ptr<Mix_Chunk> AudioManager::LoadSFX(const std::string_view& filepath)
{
	auto filePathString = std::string(filepath);
	
	std::lock_guard<std::mutex> lock(audioMutex);

	auto it = sfxCache.find(filePathString);

	if (it != sfxCache.end())
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
	
	Mix_VolumeChunk(raw, sfxVolume);

	sfxCache[filePathString] = shared;

	return shared;
}

bool AudioManager::PlayMusicTrack(const std::shared_ptr<Mix_Music>& music, int loopCount)
{
	if (music == nullptr)
		return false;

	Mix_VolumeMusic(static_cast<int>((musicVolume * masterVolume) * MIX_MAX_VOLUME));
	
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
	
	Mix_Volume(channel, static_cast<int>((sfxVolume * masterVolume) * MIX_MAX_VOLUME));

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

	Mix_VolumeMusic((musicVolume * masterVolume) / MIX_MAX_VOLUME);
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
	musicVolume = std::clamp(volume, 0.0f, 1.0f);

	Mix_VolumeMusic(static_cast<int>((musicVolume * masterVolume) * MIX_MAX_VOLUME));
}

void AudioManager::SetGlobalSFXVolume(float volume)
{
	sfxVolume = std::clamp(volume, 0.0f, 1.0f);

	SetSFXVolume(sfxVolume);
}

void AudioManager::SetSFXVolume(float volume, int sfxChannel)
{
	volume = std::clamp(volume, 0.0f, 1.0f);

	// If sfxChannel == -1, it will set the volume of all channels
	Mix_Volume(sfxChannel, static_cast<int>((volume * sfxVolume * masterVolume) * MIX_MAX_VOLUME));
}

void AudioManager::SetMasterVolume(float volume)
{
	masterVolume = std::clamp(volume, 0.0f, 1.0f);

	SetMusicVolume(musicVolume);
	SetSFXVolume(sfxVolume);
}

float AudioManager::GetMasterVolume()
{
	return masterVolume;
}

float AudioManager::GetMusicGlobalVolume()
{
	return musicVolume;
}

float AudioManager::GetGlobalSFXVolume()
{
	return sfxVolume;
}
