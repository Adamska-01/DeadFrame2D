#pragma once
#include "Core/Context/Systems/Audio/Abstractions/IAudioBackend.h"
#include "Data/Components/Collision/CollisionInfo.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/Collisions/Abstractions/ContactEventProvider.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <string>


class b2Body;
class b2Fixture;
class b2CircleShape;


namespace DF2D::Core
{
	class AudioManager;
}


namespace DF2D::Engine
{
	class Transform;
	class AudioListener;


	class DF2D_API AudioSource : public ContactEventProvider
	{
		TYPE_INFO(AudioSource, ContactEventProvider);


	protected:
		Core::AudioManager* audioManager;

		ComponentHandle<Transform> transform;

		ComponentHandle<AudioListener> audioListenerInContact;

		Data::AudioResourceID sfxClip;

		Data::AudioResourceID musicTrack;

		// Using Box2D to detect audio source collisions with audio listeners
		b2Body* collisionBody;

		b2Fixture* collisionFixture;

		bool isMusic;

		float minReachingDistance;

		float maxReachingDistance;

		float volume;

		int loops; // Number of loops (-1 for infinite)

		int playingChannel;

		Core::Vector2F lastTransformPosition;

		float lastTransformRotation;


		void OnAudioSourceEnterHandler(const Data::CollisionInfo& collisionInfo);

		void OnAudioSourceExitHandler(const Data::CollisionInfo& collisionInfo);

		void RebuildFixture();


	public:
		AudioSource();

		AudioSource(const std::string& audioSource, bool isMusic = false, float volume = 1.0f);

		~AudioSource() override;


		void Init() override;

		void Update(float deltaTime) override;

		void LateUpdate(float deltaTime) override;

		void Draw() override;


		void LoadAudio(const std::string& audioSource, bool isMusic = false);

		void Play(bool loop = false);

		void Pause();

		void Stop();


		void SetVolume(float vol);

		void SetMaxDistance(float newMaxDistance);

		void SetMinDistance(float newMinDistance);

		float GetVolume() const;
	};
}