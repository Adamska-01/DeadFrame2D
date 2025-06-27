#pragma once
#include "Components/Abstractions/ContactEventProvider.h"
#include "Data/Collision/CollisionInfo.h"
#include "Subsystems/AudioManager.h"
#include <memory>
#include <string_view>


class Transform;
class AudioListener;
class b2Body;
class b2Fixture;
class b2CircleShape;


class AudioSource : public ContactEventProvider
{
	friend class ContactListener;


protected:
	Transform* transform;

	AudioListener* audioListenerInContact;

	std::shared_ptr<Mix_Chunk> sfxClip;

	std::shared_ptr<Mix_Music> musicTrack;

	// Using Box2D to detect audio source collisions with audio listeners
	b2Body* collisionBody;
	
	b2Fixture* collisionFixture;

	bool isMusic;

	float minReachingDistance;

	float maxReachingDistance;

	float volume;

	int loops; // Number of loops (-1 for infinite)

	int playingChannel;

	Vector2F lastTransformPosition;

	float lastTransformRotation;


	void OnAudioSourceEnterHandler(const CollisionInfo& collisionInfo);

	void OnAudioSourceExitHandler(const CollisionInfo& collisionInfo);

	void RebuildFixture();


public:
	AudioSource();

	AudioSource(const std::string& audioSource, bool isMusic = false, float volume = 1.0f);

	virtual ~AudioSource() override;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void LoadAudio(const std::string_view& audioSource, bool isMusic = false);

	void Play(bool loop = false);

	void Pause();

	void Stop();


	void SetVolume(float vol);

	void SetMaxDistance(float newMaxDistance);

	void SetMinDistance(float newMinDistance);

	int GetVolume() const;
};