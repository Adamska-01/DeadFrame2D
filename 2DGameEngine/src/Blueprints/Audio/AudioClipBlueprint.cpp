#include <Components/Audio/AudioSource.h>
#include <Components/Transform.h>
#include "Blueprints/Audio/AudioClipBlueprint.h"


AudioClipBlueprint::AudioClipBlueprint(const std::string_view& audioSourcePath, Vector2F position, float volume, bool isMusic, bool loop)
	: audioSourcePath(audioSourcePath),
	position(position),
	volume(volume),
	isMusic(isMusic),
	loop(loop)
{
	transform->SetWorldPosition(position);
}

void AudioClipBlueprint::ConstructGameObject()
{
	audioSource = AddComponent<AudioSource>();

	audioSource->LoadAudio(audioSourcePath, isMusic);
	audioSource->SetVolume(volume);
	audioSource->Play(loop);
}

AudioSource* AudioClipBlueprint::GetAudioSource()
{
	return audioSource;
}