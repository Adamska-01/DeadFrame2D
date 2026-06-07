#include "Converters/Physics/PhysicsConversions.h"
#include "Core/Context/Systems/Audio/AudioManager.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Engine/ECS/Component/Audio/AudioListener.h"
#include "Engine/ECS/Component/Audio/AudioSource.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include "Utilities/Helpers/Events/EventHelpers.h"
#include <algorithm>
#include <box2d/b2_body.h>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Internal;
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	AudioSource::AudioSource()
		: audioManager(nullptr),
		sfxClip(0),
		musicTrack(0),
		collisionBody(nullptr),
		collisionFixture(nullptr),
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

		if (collisionBody == nullptr)
			return;

		PhysicsEngine2D::DestroyBody(collisionBody);
		collisionBody = nullptr;
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
		if (collisionBody == nullptr)
		{
			auto bodyDef = BodyDefinition2D
			{
				.type = BodyType2D::Kinematic,
				.gravityScale = 0.0f
			};

			auto bodyDefBox2d = Physics::ToB2BodyDef(bodyDef);

			collisionBody = PhysicsEngine2D::CreateBody(&bodyDefBox2d);
		}

		if (collisionFixture != nullptr)
		{
			collisionBody->DestroyFixture(collisionFixture);

			collisionFixture = nullptr;
		}

		auto physicsMat = PhysicsMaterial
		{
			.shape = Physics::ToB2CircleShape(maxReachingDistance),
			.isSensor = true,
			.filter = FilterData
			{
				.categoryBits = PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("AUDIO"),
				.maskBits = PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("AUDIO")
			}
		};

		auto fixtureDef = Physics::ToB2FixtureDef(physicsMat, reinterpret_cast<uintptr_t>(this));

		collisionFixture = collisionBody->CreateFixture(&fixtureDef);
		
		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();

		const auto METER_PER_PIXEL = DF2D::Core::PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

		auto angleRad = lastTransformRotation * (MathConstants::PI_f / 180.0f);

		collisionBody->SetTransform(b2Vec2(lastTransformPosition.x * METER_PER_PIXEL, lastTransformPosition.y * METER_PER_PIXEL), angleRad);

		isDirty = false;
	}

	void AudioSource::Init()
	{
		RegisterContactEnterHandler(GetHandle(), EventHelpers::BindFunction(this, &AudioSource::OnAudioSourceEnterHandler));
		RegisterContactExitHandler(GetHandle(), EventHelpers::BindFunction(this, &AudioSource::OnAudioSourceExitHandler));

		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
		audioManager = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().audioManager, NAME_OF(audioManager));

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
			currentTransformPosition *= DF2D::Core::PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

			auto angleRad = currentTransformRotation * (MathConstants::PI_f / 180.0f);

			collisionBody->SetTransform(b2Vec2(currentTransformPosition.x, currentTransformPosition.y), angleRad);
			collisionBody->SetAwake(true);
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