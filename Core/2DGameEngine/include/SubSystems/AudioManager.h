#pragma once
#include <iostream>
#include <SDL_mixer.h>
#include <vector>


enum MusicType
{
	MUSIC, 
	SFX
};



class AudioManager
{
private:
	AudioManager();


	static AudioManager* Instance;


	std::vector<Mix_Chunk*> Sfx;

	std::vector<Mix_Music*> Music;

	Mix_Music* music;

	Mix_Chunk* sfx;


public:
	inline static AudioManager* GetInstance() { return Instance = (Instance != nullptr) ? Instance : new AudioManager(); }

	
	// TODO: id is never used. Fucking use it (just return id after loading audio, remove parameter)
	bool LoadAudio(int id, std::string filename, MusicType audioType, int volume);
	
	void PlayMusicTrack(int id, int loopNumber);
	
	void PlaySFX(int id, int loopNumber, int channel);
	
	void FadeMusicTrack(int id, int loopNumber, int fadeLenght);

	void StopMusic();

	void Clean();
};