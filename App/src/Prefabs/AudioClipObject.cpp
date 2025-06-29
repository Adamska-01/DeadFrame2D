#include "Constants/AssetPaths.h"
#include "Prefabs/AudioClipObject.h"
#include <Components/Audio/AudioSource.h>
#include <Components/Transform.h>


AudioClipObject::AudioClipObject(const std::string_view& audioSourcePath, Vector2F position, float volume, bool isMusic, bool loop)
	: audioSourcePath(audioSourcePath),
	position(position),
	volume(volume),
	isMusic(isMusic),
	loop(loop)
{
	transform->SetWorldPosition(position);
}

void AudioClipObject::ConstructGameObject()
{
	audioSource = AddComponent<AudioSource>();

	audioSource->LoadAudio(audioSourcePath, isMusic);
	audioSource->SetVolume(volume);
	audioSource->Play(loop);
}

AudioSource* AudioClipObject::GetAudioSource()
{
	return audioSource;
}
