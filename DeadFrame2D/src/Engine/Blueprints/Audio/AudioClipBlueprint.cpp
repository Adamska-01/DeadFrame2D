#include "Engine/Blueprints/Audio/AudioClipBlueprint.h"
#include "Engine/Components/Audio/AudioSource.h"
#include "Engine/Components/Transform.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


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
}