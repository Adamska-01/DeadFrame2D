#pragma once
#include <iostream>
#include <SDL_mixer.h>
#include <vector>


class AudioManager
{
private:
	AudioManager() = default;

	AudioManager(const AudioManager&) = delete;

	AudioManager(AudioManager&&) = delete;


	AudioManager& operator=(const AudioManager&) = delete;

	AudioManager& operator=(AudioManager&&) = delete;


public:
	static Mix_Music* LoadMusic(const char* filepath);
	
	static Mix_Chunk* LoadSFX(const char* filepath, int volume);

	static bool PlayMusicTrack(Mix_Music* music, int loopNumber);

	static void PlaySFX(Mix_Chunk* sfx, int loopNumber);
	
	static void FadeInMusicTrack(Mix_Music* music, int loopNumber, int fadeLenght);

	static void StopMusic();
};