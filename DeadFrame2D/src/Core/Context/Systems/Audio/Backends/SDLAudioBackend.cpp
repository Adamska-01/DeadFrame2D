#include "Core/Context/Systems/Audio/Backends/SDLAudioBackend.h"
#include <iostream>
#include <SDL_mixer.h>


namespace DF2D::Core
{
	SDLAudioBackend::SDLAudioBackend(int maxSFXChannelAllocation)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;

			return;
		}

		Mix_AllocateChannels(maxSFXChannelAllocation);

		std::cout << "[Info] SDL_Mixer subsystem successfully initialized." << std::endl;
	}

	SDLAudioBackend::~SDLAudioBackend()
	{
		Mix_CloseAudio();

		std::cout << "[Info] SDL_Mixer subsystem successfully quit." << std::endl;
	}


	Mix_Music* SDLAudioBackend::LoadMusic(const std::string& path)
	{
		auto raw = Mix_LoadMUS(path.c_str());

		if (raw == nullptr)
		{
			std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
		}

		return raw;
	}

	Mix_Chunk* SDLAudioBackend::LoadSFX(const std::string& path)
	{
		auto raw = Mix_LoadWAV(path.c_str());

		if (raw == nullptr)
		{
			std::cerr << "Failed to load SFX: " << Mix_GetError() << std::endl;
		}

		return raw;
	}

	bool SDLAudioBackend::PlayMusic(Mix_Music* music, int loops)
	{
		return Mix_PlayMusic(music, loops) == 0;
	}

	int SDLAudioBackend::PlayChannel(int channel, Mix_Chunk* chunk, int loops)
	{
		return Mix_PlayChannel(channel, chunk, loops);
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

	void SDLAudioBackend::SetMusicVolume(int volume)
	{
		Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
	}

	void SDLAudioBackend::SetChannelVolume(int channel, int volume)
	{
		// If sfxChannel == -1, it will set the volume of all channels
		Mix_Volume(channel, volume * MIX_MAX_VOLUME);
	}

	bool SDLAudioBackend::IsMusicPlaying()
	{
		return Mix_PlayingMusic();
	}
}