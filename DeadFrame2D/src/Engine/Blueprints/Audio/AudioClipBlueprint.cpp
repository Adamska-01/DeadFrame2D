#include "Engine/Blueprints/Audio/AudioClipBlueprint.h"
#include "Engine/ECS/Component/Audio/AudioSource.h"
#include "Engine/ECS/Component/Transform.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;


	AudioClipBlueprint::AudioClipBlueprint(const std::string_view& audioSourcePath, Vector2F position, float volume, bool isMusic, bool loop)
		: audioSourcePath(audioSourcePath),
		position(position),
		volume(volume),
		isMusic(isMusic),
		loop(loop)
	{
	}

	void AudioClipBlueprint::ConstructGameObject()
	{
		transform->SetWorldPosition(position);

		audioSource = AddComponent<AudioSource>();

		audioSource->LoadAudio(std::string(audioSourcePath), isMusic);
		audioSource->Play(loop);
		audioSource->SetVolume(volume);
	}

	ComponentHandle<AudioSource> AudioClipBlueprint::GetAudioSource()
	{
		return audioSource;
	}
}