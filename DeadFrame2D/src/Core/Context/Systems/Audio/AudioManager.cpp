#include "Core/Context/Systems/Audio/AudioManager.h"
#include <algorithm>
#include <algorithm>
#include <cassert>
#include <iostream>


namespace DF2D::Core
{
	using namespace DF2D::Models;


	AudioManager::AudioManager(AudioConfig audioConfig, std::unique_ptr<IAudioBackend> backend)
		: audioConfig(audioConfig), 
		backend(std::move(backend))
	{
	}


	std::shared_ptr<Mix_Music> AudioManager::LoadMusic(const std::string& filePath)
	{
		std::lock_guard lock(mutex);

		auto it = musicCache.find(filePath);

		if (it != musicCache.end())
		{
			auto locked = it->second.lock();

			if (locked != nullptr)
				return locked;
		}

		auto raw = backend->LoadMusic(filePath);

		if (raw == nullptr)
			return nullptr;

		auto shared = std::shared_ptr<Mix_Music>(raw, Mix_FreeMusic);

		musicCache[filePath] = shared;

		return shared;
	}

	std::shared_ptr<Mix_Chunk> AudioManager::LoadSFX(const std::string& filePath)
	{
		std::lock_guard lock(mutex);

		auto it = sfxCache.find(filePath);

		if (it != sfxCache.end())
		{
			auto locked = it->second.lock();

			if (locked != nullptr)
				return locked;
		}

		auto raw = backend->LoadSFX(filePath);

		if (raw == nullptr)
			return nullptr;

		auto shared = std::shared_ptr<Mix_Chunk>(raw, Mix_FreeChunk);

		sfxCache[filePath] = shared;

		return shared;
	}

	bool AudioManager::PlayMusics(const std::shared_ptr<Mix_Music>& music, int loops)
	{
		if (music == nullptr)
			return false;

		backend->SetMusicVolume(static_cast<int>(audioConfig.masterVolume * audioConfig.musicVolume));

		return backend->PlayMusic(music.get(), loops);
	}

	int AudioManager::PlaySFX(const std::shared_ptr<Mix_Chunk>& sfx, int loops)
	{
		if (sfx == nullptr)
			return -1;

		auto channel = backend->PlayChannel(-1, sfx.get(), loops);

		if (channel < 0)
			return -1;

		backend->SetChannelVolume(channel, static_cast<int>(audioConfig.masterVolume * audioConfig.sfxVolume));

		return channel;
	}

	void AudioManager::StopMusic()
	{
		backend->StopMusic();
	}

	void AudioManager::StopChannel(int c)
	{
		backend->StopChannel(c);
	}

	void AudioManager::PauseMusic()
	{
		backend->PauseMusic();
	}

	void AudioManager::ResumeMusic()
	{
		backend->ResumeMusic();
	}

	void AudioManager::SetMasterVolume(float v)
	{
		audioConfig.masterVolume = std::clamp(v, 0.0f, 1.0f);

		backend->SetMusicVolume(audioConfig.musicVolume);
		backend->SetChannelVolume(-1, audioConfig.sfxVolume);
	}

	void AudioManager::SetMusicVolume(float volume)
`	{
		audioConfig.musicVolume = std::clamp(volume, 0.0f, 1.0f);

		backend->SetMusicVolume(static_cast<int>(audioConfig.masterVolume * audioConfig.musicVolume));
	}
	
	void AudioManager::SetSFXVolume(float volume, int sfxChannel)
	{
		volume = std::clamp(volume, 0.0f, 1.0f);

		if (sfxChannel == -1)
		{
			audioConfig.sfxVolume = volume;
		}
		else
		{
			volume *= audioConfig.sfxVolume;
		}

		auto sfxVolume = audioConfig.sfxVolume;
		auto masterVolume = audioConfig.masterVolume;

		backend->SetChannelVolume(sfxChannel, static_cast<int>(audioConfig.masterVolume * volume));
	}

	float AudioManager::GetMasterVolume() const
	{
		return audioConfig.masterVolume;
	}

	float AudioManager::GetMusicVolume() const
	{
		return audioConfig.musicVolume;
	}

	float AudioManager::GetSFXVolume() const
	{
		return audioConfig.sfxVolume;
	}
}