#include "Core/Context/Systems/Audio/AudioManager.h"
#include <algorithm>


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Models;


	AudioManager::AudioManager(AudioConfig audioConfig, std::unique_ptr<IAudioBackend> backend)
		: audioConfig(audioConfig),
		backend(std::move(backend))
	{
	}


	void AudioManager::BeginFrame()
	{
	}

	void AudioManager::PreUpdate(float deltaTime)
	{
	}

	void AudioManager::EndUpdate(float deltaTime)
	{
	}

	void AudioManager::EndDraw()
	{
	}


	AudioResourceID AudioManager::LoadMusic(const std::string& filePath)
	{
		std::lock_guard lock(mutex);

		auto it = musicCache.find(filePath);

		if (it != musicCache.end())
			return it->second;

		auto id = backend->LoadMusic(filePath);

		if (id != -1)
		{
			musicCache[filePath] = id;
		}

		return id;
	}

	AudioResourceID AudioManager::LoadSFX(const std::string& filePath)
	{
		std::lock_guard lock(mutex);

		auto it = sfxCache.find(filePath);

		if (it != sfxCache.end())
			return it->second;

		auto id = backend->LoadSFX(filePath);

		if (id != 0)
			sfxCache[filePath] = id;

		return id;
	}

	bool AudioManager::PlayMusics(AudioResourceID music, int loops)
	{
		if (music < 0)
			return false;

		backend->SetMusicVolume(audioConfig.masterVolume * audioConfig.musicVolume);

		return backend->PlayMusic(music, loops);
	}

	int AudioManager::PlaySFX(AudioResourceID sfx, int loops)
	{
		if (sfx < 0)
			return -1;

		auto channel = backend->PlayChannel(-1, sfx, loops);

		if (channel < 0)
			return -1;

		backend->SetChannelVolume(channel, audioConfig.masterVolume * audioConfig.sfxVolume);

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

	void AudioManager::PauseChannel(int channel)
	{
		backend->PauseChannel(channel);
	}

	void AudioManager::ResumeChannel(int channel)
	{
		backend->ResumeChannel(channel);
	}

	bool AudioManager::IsMusicPlaying()
	{
		return backend->IsMusicPlaying();
	}

	void AudioManager::SetMasterVolume(float v)
	{
		audioConfig.masterVolume = std::clamp(v, 0.0f, 1.0f);

		backend->SetMusicVolume(audioConfig.masterVolume * audioConfig.musicVolume);
		backend->SetChannelVolume(-1, audioConfig.masterVolume * audioConfig.sfxVolume);
	}

	void AudioManager::SetMusicVolume(float volume)
	{
		audioConfig.musicVolume = std::clamp(volume, 0.0f, 1.0f);

		backend->SetMusicVolume(audioConfig.masterVolume * audioConfig.musicVolume);
	}

	void AudioManager::SetSFXVolume(float volume, int sfxChannel)
	{
		volume = std::clamp(volume, 0.0f, 1.0f);

		if (sfxChannel == -1)
		{
			audioConfig.sfxVolume = volume;

			backend->SetChannelVolume(-1, audioConfig.masterVolume * audioConfig.sfxVolume);
		}
		else
		{
			backend->SetChannelVolume(sfxChannel, audioConfig.masterVolume * audioConfig.sfxVolume * volume);
		}
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