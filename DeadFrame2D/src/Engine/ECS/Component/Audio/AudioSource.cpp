#include "Core/Context/Systems/Audio/AudioManager.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Engine/ECS/Component/Audio/AudioListener.h"
#include "Engine/ECS/Component/Audio/AudioSource.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include "Utilities/Helpers/Events/EventHelpers.h"
#include <algorithm>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	AudioSource::AudioSource()
		: audioManager(nullptr),
		physicsEngine(nullptr),
		sfxClip(0),
		musicTrack(0),
		collisionBody(0),
		collisionFixture(0),
		isMusic(false),
		minReachingDistance(1),
		maxReachingDistance(9999.0f),
		volume(1.0f),
		loops(0),
		playingChannel(-1),
		lastTransformPosition(Vector2F::Zero),
		lastTransformRotation(0.0f)
	{
	}

	AudioSource::AudioSource(const std::string& audioSource, bool isMusic, float volume)
		: AudioSource()
	{
		this->isMusic = isMusic;
		this->volume = std::clamp(volume, 0.0f, 1.0f);

		LoadAudio(audioSource, isMusic);
	}

	AudioSource::~AudioSource()
	{
		DeregisterContactEnterHandler(GetHandle());
		DeregisterContactExitHandler(GetHandle());

		if (audioManager != nullptr)
		{
			Stop();
		}

		if (physicsEngine == nullptr || collisionBody == 0)
			return;

		physicsEngine->DestroyBody(collisionBody);
		collisionBody = 0;
	}

	void AudioSource::OnAudioSourceEnterHandler(const CollisionInfo& collisionInfo)
	{
		if (collisionInfo.otherGameObject == nullptr)
			return;

		auto audioListener = collisionInfo.otherGameObject->GetComponent<AudioListener>();

		if (audioListener == nullptr)
			return;

		audioListenerInContact = audioListener;
	}

	void AudioSource::OnAudioSourceExitHandler(const CollisionInfo& collisionInfo)
	{
		if (collisionInfo.otherGameObject == nullptr)
			return;

		auto audioListener = collisionInfo.otherGameObject->GetComponent<AudioListener>();

		if (audioListener == nullptr)
			return;

		audioListenerInContact = nullptr;
	}

	void AudioSource::RebuildFixture()
	{
		if (collisionBody == 0)
		{
			auto bodyDef = BodyDefinition2D
			{
				.type = BodyType2D::Kinematic,
				.gravityScale = 0.0f
			};

			collisionBody = physicsEngine->CreateBody(bodyDef);
		}

		if (collisionFixture != 0)
		{
			physicsEngine->DestroyFixture(collisionFixture);

			collisionFixture = 0;
		}

		auto physicsMat = PhysicsMaterial
		{
			.shape = CircleShapeDefinition2D
			{
				.radius = maxReachingDistance
			},
			.isSensor = true,
			.filter = FilterData
			{
				.categoryBits = physicsEngine->GetCollisionMasks().GetMaskFlagByName("AUDIO"),
				.maskBits = physicsEngine->GetCollisionMasks().GetMaskFlagByName("AUDIO")
			}
		};

		collisionFixture = physicsEngine->CreateFixture(collisionBody, physicsMat, GetHandleAs<ContactEventProvider>());

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();

		auto angleRad = lastTransformRotation * (MathConstants::PI_f / 180.0f);

		physicsEngine->SetBodyTransform(collisionBody, lastTransformPosition, angleRad);

		isDirty = false;
	}

	void AudioSource::Init()
	{
		RegisterContactEnterHandler(GetHandle(), EventHelpers::BindFunction(this, &AudioSource::OnAudioSourceEnterHandler));
		RegisterContactExitHandler(GetHandle(), EventHelpers::BindFunction(this, &AudioSource::OnAudioSourceExitHandler));

		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
		audioManager = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().audioManager, NAME_OF(audioManager));
		physicsEngine = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().physicsEngine, NAME_OF(physicsEngine));

		isDirty = true;
	}

	void AudioSource::Update(float deltaTime)
	{
		if (isDirty)
		{
			RebuildFixture();
		}
	}

	void AudioSource::LateUpdate(float deltaTime)
	{
		auto safeDelta = std::max(deltaTime, std::numeric_limits<float>::epsilon());

		auto currentTransformPosition = transform->GetWorldPosition();
		auto currentTransformRotation = transform->GetWorldRotation();

		if (currentTransformPosition != lastTransformPosition || currentTransformRotation != lastTransformRotation)
		{
			auto angleRad = currentTransformRotation * (MathConstants::PI_f / 180.0f);

			physicsEngine->SetBodyTransform(collisionBody, currentTransformPosition, angleRad);
			physicsEngine->SetBodyAwake(collisionBody, true);
		}

		if (playingChannel == -1)
			return;

		// Audio attenuation logic 
		if (audioListenerInContact == nullptr || sfxClip == 0)
			return;

		auto listenerTransform = audioListenerInContact->GetGameObject()->GetTransform();

		if (listenerTransform == nullptr)
			return;

		auto sourcePos = transform->GetWorldPosition();
		auto listenerPos = listenerTransform->GetWorldPosition();

		auto distance = sourcePos.Distance(listenerPos);

		auto distanceRange = maxReachingDistance - minReachingDistance;
		auto t = distanceRange > 0.0001f
			? std::clamp((distance - minReachingDistance) / distanceRange, 0.0f, 1.0f)
			: (distance <= minReachingDistance ? 0.0f : 1.0f);

		auto attenuatedVolume = (1.0f - t) * volume;

		audioManager->SetSFXVolume(attenuatedVolume, playingChannel);
	}

	void AudioSource::Draw()
	{
		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();
	}

	void AudioSource::LoadAudio(const std::string& audioSource, bool isMusic)
	{
		Stop();

		this->isMusic = isMusic;

		if (isMusic)
		{
			musicTrack = audioManager->LoadMusic(audioSource);
		}
		else
		{
			sfxClip = audioManager->LoadSFX(audioSource);
		}
	}

	void AudioSource::Play(bool loop)
	{
		Stop();

		if (isMusic && musicTrack != 0)
		{
			audioManager->PlayMusics(musicTrack, loop ? -1 : 0);
			audioManager->SetMusicVolume(volume);
		}
		else if (sfxClip != 0)
		{
			playingChannel = audioManager->PlaySFX(sfxClip, loop ? -1 : 0);
			audioManager->SetSFXVolume(volume, playingChannel);
		}
	}

	void AudioSource::Pause()
	{
		if (isMusic)
		{
			audioManager->PauseMusic();
		}
		else if (playingChannel != -1)
		{
			audioManager->PauseChannel(playingChannel);
		}
	}

	void AudioSource::Stop()
	{
		if (isMusic)
		{
			audioManager->StopMusic();
		}
		else if (playingChannel != -1)
		{
			audioManager->StopChannel(playingChannel);
		}
	}

	void AudioSource::SetVolume(float vol)
	{
		volume = std::clamp(vol, 0.0f, 1.0f);

		if (isMusic)
		{
			audioManager->SetMusicVolume(vol);
		}
		else if (playingChannel != -1)
		{
			audioManager->SetSFXVolume(vol, playingChannel);
		}
	}

	void AudioSource::SetMaxDistance(float newMaxDistance)
	{
		maxReachingDistance	= newMaxDistance;

		isDirty = true;
	}

	void AudioSource::SetMinDistance(float newMinDistance)
	{
		minReachingDistance = newMinDistance;
	
		isDirty = true;
	}

	float AudioSource::GetVolume() const
	{
		return volume;
	}
}