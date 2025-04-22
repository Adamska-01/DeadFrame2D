#include <Debugging/Debug.h>
#include <iostream>
#include <SubSystems/AudioManager.h>


AudioManager::AudioManager()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0)
	{
		std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;

		return;
	}

	std::cout << "[Info] SDL_Mixer successfully initialized." << std::endl;
}

AudioManager::~AudioManager()
{
	// TODO: Implementing a music/sfx cache is a good idea (use shared_ptr)

	Mix_CloseAudio();

	std::cout << "[Info] SDL_Mixer subsystem successfully quit." << std::endl;
}

Mix_Music* AudioManager::LoadMusic(const char* filepath)
{
	auto music = Mix_LoadMUS(filepath);

	if (music == nullptr) 
	{
		std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
	}

	return music;
}

Mix_Chunk* AudioManager::LoadSFX(const char* filepath, int volume)
{
	auto sfx = Mix_LoadWAV(filepath);

	if (sfx == nullptr)
	{
		std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
	}

	Mix_VolumeChunk(sfx, volume);

	return sfx;
}

bool AudioManager::PlayMusicTrack(Mix_Music* music, int loopNumber)
{
	return Mix_PlayMusic(music, loopNumber) == 0;
}

void AudioManager::PlaySFX(Mix_Chunk* sfx, int loopNumber)
{
	auto channel = 0;

	while (Mix_Playing(channel))
	{
		++channel;
	}

	Mix_PlayChannel(channel, sfx, loopNumber);
}

void AudioManager::FadeInMusicTrack(Mix_Music* music, int loopNumber, int fadeLenght)
{
	if (Mix_PlayingMusic() != 0)
	{
		Mix_FadeOutMusic(fadeLenght);
	}

	// Fade in milliseconds
	Mix_FadeInMusic(music, loopNumber, fadeLenght);
}

void AudioManager::StopMusic()
{
	Mix_HaltMusic();
}