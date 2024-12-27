#include <Debugging/Debug.h>
#include <SubSystems/AudioManager.h>


AudioManager* AudioManager::Instance = nullptr;


AudioManager::AudioManager()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0) 
	{
		std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

bool AudioManager::LoadAudio(int id, std::string filename, MusicType audioType, int volume)
{
	switch (audioType)
	{
		case MUSIC:
			music = Mix_LoadMUS(filename.c_str());

			if (!music)
			{
				std::cout << "Unable to load music file: " << Mix_GetError() << std::endl;
				return false;
			}
		
			Mix_VolumeMusic(volume);
			Music.push_back(music);
		
			return true;

		case SFX:
			sfx = Mix_LoadWAV(filename.c_str());
		
			if (!sfx)
			{
				std::cout << "Unable to load SFX file: " << Mix_GetError() << std::endl;
				return false;
			}
		
			Mix_VolumeChunk(sfx, volume);
			Sfx.push_back(sfx);
		
			return true;

		default:
			std::cout << "Unknown audio type!" << std::endl;
			return false;
	}
}

void AudioManager::PlayMusicTrack(int id, int loopNumber)
{
	Mix_PlayMusic(Music[id], loopNumber);
}

void AudioManager::PlaySFX(int id, int loopNumber, int channel)
{
	while (Mix_Playing(channel))
	{
		++channel;
	}

	// -1 checking every channel
	if (!Mix_Playing(channel)) 
	{
		// -1 to loop continuously
		Mix_PlayChannel(channel, Sfx[id], loopNumber); 
	}
}

void AudioManager::FadeMusicTrack(int id, int loopNumber, int fadeLenght)
{
	if (Mix_PlayingMusic())
	{
		Mix_FadeOutMusic(fadeLenght);
	}

	// Fade in milliseconds
	Mix_FadeInMusic(Music[id], loopNumber, fadeLenght); 
}

void AudioManager::StopMusic()
{
	Mix_HaltMusic();
}

void AudioManager::Clean()
{
	if (&Music != nullptr)
	{
		for (auto i = 0; i < Music.size(); i++)
		{
			Mix_FreeMusic(Music[i]);
		}

		Music.clear();
	}
	if (&Sfx != nullptr)
	{
		for (auto i = 0; i < Sfx.size(); i++)
		{
			Mix_FreeChunk(Sfx[i]);
		}
		
		Sfx.clear();
	}

	//No need to delete the holders
	music = nullptr;
	sfx = nullptr;

#if _DEBUG
	DBG_ASSERT_MSG_EMPTY(Music.size(), "DEBUG_MSG: Music cleaned! \n");
	DBG_ASSERT_MSG_EMPTY(Sfx.size(), "DEBUG_MSG: SFX cleaned! \n");
#endif

	Mix_CloseAudio();
}
