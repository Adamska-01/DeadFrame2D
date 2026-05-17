#pragma once
#include "Core/Context/Systems/Audio/Abstractions/IAudioBackend.h"


struct MockAudioBackend : DF2D::Core::IAudioBackend
{
	// Configurable results
	DF2D::Data::AudioResourceID nextMusicId{1};
	DF2D::Data::AudioResourceID nextSfxId{1};
	bool playMusicResult{true};
	int playChannelStart{0};

	// Call tracking
	int loadMusicCount{0};
	int loadSFXCount{0};
	int playMusicCount{0};
	int playChannelCount{0};
	DF2D::Data::AudioResourceID lastPlayedMusic{0};
	DF2D::Data::AudioResourceID lastPlayedSFX{0};
	float lastMusicVolume{0};
	float lastSFXVolume{0};
	int lastPausedChannel{-1};
	int lastResumedChannel{-1};
	int lastStoppedChannel{-1};
	int lastStoppedMusicCount{0};
	int lastPausedMusicCount{0};
	int lastResumedMusicCount{0};
	bool isMusicPlayingResult{false};


	DF2D::Data::AudioResourceID LoadMusic(const std::string&) override
	{
		loadMusicCount++;

		return nextMusicId++;
	}

	DF2D::Data::AudioResourceID LoadSFX(const std::string&) override
	{
		loadSFXCount++;

		return nextSfxId++;
	}

	bool PlayMusic(DF2D::Data::AudioResourceID id, int) override
	{
		lastPlayedMusic = id;
		playMusicCount++;
		isMusicPlayingResult = true;

		return playMusicResult;
	}

	int PlayChannel(int, DF2D::Data::AudioResourceID id, int) override
	{
		lastPlayedSFX = id;
		playChannelCount++;

		return playChannelStart++;
	}

	void StopMusic() override
	{
		lastStoppedMusicCount++;
		isMusicPlayingResult = false;
	}

	void PauseMusic() override
	{
		lastPausedMusicCount++;
		isMusicPlayingResult = false;
	}

	void ResumeMusic() override
	{
		lastResumedMusicCount++;
		isMusicPlayingResult = true;
	}

	void StopChannel(int c) override
	{
		lastStoppedChannel = c;
	}

	void PauseChannel(int c) override
	{
		lastPausedChannel = c;
	}

	void ResumeChannel(int c) override
	{
		lastResumedChannel = c;
	}

	void SetMusicVolume(float v) override
	{
		lastMusicVolume = v;
	}

	void SetChannelVolume(int, float v) override
	{
		lastSFXVolume = v;
	}

	bool IsMusicPlaying() override
	{
		return isMusicPlayingResult;
	}
};