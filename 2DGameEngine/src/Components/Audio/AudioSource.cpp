#include "Components/Audio/AudioListener.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "Tools/Helpers/Physics/PhysicsShapeCreators.h"
#include <algorithm>
#include <box2d/b2_body.h>
#include <SubSystems/Physics/PhysicsEngine2D.h>
#include <Tools/Helpers/EventHelpers.h>
#include <Tools/Helpers/Physics/PhysicsConversion.h>


AudioSource::AudioSource()
	: transform(nullptr),
	audioListenerInContact(nullptr),
	collisionBody(nullptr),
	collisionFixture(nullptr),
	isMusic(false),
	minReachingDistance(1),
	maxReachingDistance(std::numeric_limits<float>::max()),
	volume(1.0f),
	loops(0),
	playingChannel(-1),
	lastTransformPosition(Vector2F::Zero),
	lastTransformRotation(0.0f)
{
	RegisterContactEnterHandler(EventHelpers::BindFunction(this, &AudioSource::OnAudioSourceEnterHandler), reinterpret_cast<uintptr_t>(this));
	RegisterContactExitHandler(EventHelpers::BindFunction(this, &AudioSource::OnAudioSourceExitHandler), reinterpret_cast<uintptr_t>(this));
}

AudioSource::AudioSource(const std::string& audioSource, bool isMusic, float volume)
	: AudioSource()
{
	this->isMusic = isMusic;
	this->volume = std::clamp(volume, 0.0f, 1.0f);

	LoadAudio(audioSource, isMusic);

	RegisterContactEnterHandler(EventHelpers::BindFunction(this, &AudioSource::OnAudioSourceEnterHandler), reinterpret_cast<uintptr_t>(this));
	RegisterContactExitHandler(EventHelpers::BindFunction(this, &AudioSource::OnAudioSourceExitHandler), reinterpret_cast<uintptr_t>(this));
}

AudioSource::~AudioSource()
{
	if (collisionBody == nullptr)
		return;

	auto fixture = collisionBody->GetFixtureList();

	while (fixture != nullptr)
	{
		auto next = fixture->GetNext();

		collisionBody->DestroyFixture(fixture);

		fixture = next;
	}

	DeregisterContactEnterHandler(reinterpret_cast<uintptr_t>(this));
	DeregisterContactExitHandler(reinterpret_cast<uintptr_t>(this));
}

void AudioSource::OnAudioSourceEnterHandler(const CollisionInfo& collisionInfo)
{
	if (collisionInfo.otherGameObject.expired())
		return;

	auto audioListener = collisionInfo.otherGameObject.lock()->GetComponent<AudioListener>();

	if (audioListener == nullptr)
		return;

	audioListenerInContact = audioListener;
}

void AudioSource::OnAudioSourceExitHandler(const CollisionInfo& collisionInfo)
{
	if (collisionInfo.otherGameObject.expired())
		return;

	auto audioListener = collisionInfo.otherGameObject.lock()->GetComponent<AudioListener>();

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

		auto bodyDefBox2d = PhysicsConversion::ToB2BodyDef(bodyDef);

		collisionBody = PhysicsEngine2D::CreateBody(&bodyDefBox2d);
	}

	if (collisionFixture != nullptr)
	{
		collisionBody->DestroyFixture(collisionFixture);

		collisionFixture = nullptr;
	}

	auto physicsMat = PhysicsMaterial
	{
		.shape = PhysicsShapeCreators::CreateCircleShape(maxReachingDistance),
		.isSensor = true
	};

	auto fixtureDef = PhysicsConversion::ToB2FixtureDef(physicsMat, reinterpret_cast<uintptr_t>(this));

	collisionFixture = collisionBody->CreateFixture(&fixtureDef);

	lastTransformPosition = transform->GetWorldPosition();
	lastTransformRotation = transform->GetWorldRotation();

	collisionBody->SetTransform(b2Vec2(lastTransformPosition.x * PhysicsConstants::PIXEL_TO_METER, lastTransformPosition.y * PhysicsConstants::PIXEL_TO_METER), lastTransformRotation * (MathConstants::PI / 180.0f));

	isDirty = false;
}

void AudioSource::Init()
{
	transform = OwningObject.lock()->GetTransform();

	isDirty = true;
}

void AudioSource::Start()
{

}

void AudioSource::Update(float deltaTime)
{
	if (isDirty)
		RebuildFixture();

	// TODO: Use LateUpdate for this!!!
	auto safeDelta = std::max(deltaTime, std::numeric_limits<float>::epsilon());

	// Convert positions from pixels to meters
	auto currentTransformPosition = transform->GetWorldPosition();
	auto posNow = b2Vec2(
		currentTransformPosition.x * PhysicsConstants::PIXEL_TO_METER,
		currentTransformPosition.y * PhysicsConstants::PIXEL_TO_METER);

	auto posLast = b2Vec2(
		lastTransformPosition.x * PhysicsConstants::PIXEL_TO_METER,
		lastTransformPosition.y * PhysicsConstants::PIXEL_TO_METER);

	// Velocity = (position_now - position_last) / deltaTime
	auto velocity = posNow - posLast;
	velocity.x /= safeDelta;
	velocity.y /= safeDelta;

	// Angular velocity in radians/sec
	auto currentRotationRadians = transform->GetWorldRotation() * (MathConstants::PI / 180.0f);
	auto lastRotationRadians = lastTransformRotation * (MathConstants::PI / 180.0f);
	auto angularVelocity = (currentRotationRadians - lastRotationRadians) / safeDelta;

	collisionBody->SetLinearVelocity(velocity);
	collisionBody->SetAngularVelocity(angularVelocity);

	// Audio attenuation logic unchanged...
	if (audioListenerInContact == nullptr || sfxClip == nullptr)
		return;

	auto listenerTransform = audioListenerInContact->GetGameObject().lock()->GetTransform();
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

	if (playingChannel != -1)
	{
		AudioManager::SetSFXVolume(attenuatedVolume, playingChannel);
	}
}

void AudioSource::Draw()
{
	auto pos = collisionBody->GetPosition();
	auto angle = collisionBody->GetAngle();

	transform->SetWorldPosition(Vector2F(pos.x, pos.y) * PhysicsConstants::METER_TO_PIXEL);
	transform->SetLocalRotation(angle * (180.0f / MathConstants::PI));

	lastTransformPosition = transform->GetWorldPosition();
	lastTransformRotation = transform->GetWorldRotation();
}

void AudioSource::LoadAudio(const std::string_view& audioSource, bool isMusic)
{
	Stop();

	this->isMusic = isMusic;

	if (isMusic)
	{
		musicTrack = AudioManager::LoadMusic(audioSource);
	}
	else
	{
		sfxClip = AudioManager::LoadSFX(audioSource);
	}

	// Could handle errors here
}

void AudioSource::Play(bool loop)
{
	Stop();

	if (isMusic && musicTrack != nullptr)
	{
		AudioManager::PlayMusicTrack(musicTrack, loop ? -1 : 0);
		AudioManager::SetMusicVolume(volume);
	}
	else if (sfxClip)
	{
		playingChannel = AudioManager::PlaySFX(sfxClip, loop ? -1 : 0);
		AudioManager::SetSFXVolume(0.0f, playingChannel);
	}
}

void AudioSource::Pause()
{
	if (isMusic)
	{
		AudioManager::PauseMusic();
	}
	else if (playingChannel != -1)
	{
		AudioManager::PauseSFX(playingChannel);
	}
}

void AudioSource::Stop()
{
	if (isMusic)
	{
		AudioManager::StopMusic();
	}
	else if (playingChannel != -1)
	{
		AudioManager::StopSFX(playingChannel);
	}
}

void AudioSource::SetVolume(float vol)
{
	volume = std::clamp(vol, 0.0f, 1.0f);

	if (isMusic)
	{
		AudioManager::SetMusicVolume(vol);
	}
	else if (playingChannel != -1)
	{
		AudioManager::SetSFXVolume(vol, playingChannel);
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

int AudioSource::GetVolume() const
{
	return volume;
}