#include "Factories/Products/Context/Systems/Audio/SDLAudioBackend.h"
#include <algorithm>
#include <iostream>


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Models;


	SDLAudioBackend::SDLAudioBackend(AudioConfig config)
		: nextID(1)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;

			return;
		}

		Mix_AllocateChannels(config.maxSFXChannelAllocation);

		std::cout << "[Info] SDL_Mixer subsystem successfully initialized." << std::endl;
	}

	SDLAudioBackend::~SDLAudioBackend()
	{
		musicResources.clear();
		sfxResources.clear();

		Mix_CloseAudio();

		std::cout << "[Info] SDL_Mixer subsystem successfully quit." << std::endl;
	}


	AudioResourceID SDLAudioBackend::LoadMusic(const std::string& path)
	{
		auto raw = Mix_LoadMUS(path.c_str());

		if (raw == nullptr)
		{
			std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;

			return 0;
		}

		auto id = nextID++;

		musicResources[id] = std::shared_ptr<Mix_Music>(raw, Mix_FreeMusic);

		return id;
	}

	AudioResourceID SDLAudioBackend::LoadSFX(const std::string& path)
	{
		auto raw = Mix_LoadWAV(path.c_str());

		if (raw == nullptr)
		{
			std::cerr << "Failed to load SFX: " << Mix_GetError() << std::endl;

			return 0;
		}

		auto id = nextID++;

		sfxResources[id] = std::shared_ptr<Mix_Chunk>(raw, Mix_FreeChunk);

		return id;
	}

	bool SDLAudioBackend::PlayMusic(AudioResourceID music, int loops)
	{
		auto it = musicResources.find(music);

		if (it == musicResources.end())
			return false;

		return Mix_PlayMusic(it->second.get(), loops) == 0;
	}

	int SDLAudioBackend::PlayChannel(int channel, AudioResourceID sfx, int loops)
	{
		auto it = sfxResources.find(sfx);

		if (it == sfxResources.end())
			return -1;

		return Mix_PlayChannel(channel, it->second.get(), loops);
	}

	void SDLAudioBackend::StopMusic()
	{
		Mix_HaltMusic();
	}

	void SDLAudioBackend::StopChannel(int c)
	{
		Mix_HaltChannel(c);
	}

	void SDLAudioBackend::PauseMusic()
	{
		Mix_PauseMusic();
	}

	void SDLAudioBackend::ResumeMusic()
	{
		Mix_ResumeMusic();
	}

	void SDLAudioBackend::PauseChannel(int channel)
	{
		Mix_Pause(channel);
	}

	void SDLAudioBackend::ResumeChannel(int channel)
	{
		Mix_Resume(channel);
	}

	void SDLAudioBackend::SetMusicVolume(float volume)
	{
		volume = std::clamp(volume, 0.0f, 1.0f);

		Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
	}

	void SDLAudioBackend::SetChannelVolume(int channel, float volume)
	{
		volume = std::clamp(volume, 0.0f, 1.0f);

		Mix_Volume(channel, static_cast<int>(volume * MIX_MAX_VOLUME));
	}

	bool SDLAudioBackend::IsMusicPlaying()
	{
		return Mix_PlayingMusic();
	}
}