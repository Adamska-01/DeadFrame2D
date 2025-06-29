#pragma once
#include "Generic/Bobble/BobbleColor.h"
#include <GameObject.h>
#include <Math/Vector2.h>
#include <string_view>


class AudioSource;


class AudioClipObject : public GameObject
{
private:
	AudioSource* audioSource;

	std::string_view audioSourcePath;
	
	Vector2F position;
	
	float volume;
	
	bool isMusic;

	bool loop;


public:
	AudioClipObject(
		const std::string_view& audioSourcePath, 
		Vector2F position = Vector2F::Zero, 
		float volume = 1.0f, 
		bool isMusic = false,
		bool loop = false);


	virtual void ConstructGameObject() override;


	AudioSource* GetAudioSource();
};