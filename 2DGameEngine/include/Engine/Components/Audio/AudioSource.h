#pragma once
#include "Core/SubSystems/Systems/AudioManager.h"
#include "Data/Collision/CollisionInfo.h"
#include "Engine/Components/Collisions/Abstractions/ContactEventProvider.h"
#include <memory>
#include <string_view>


class b2Body;
class b2Fixture;
class b2CircleShape;


namespace DeadFrame2D::Engine
{
	class Transform;
	class AudioListener;


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

		DeadFrame2D::Core::Vector2F lastTransformPosition;

		float lastTransformRotation;


		void OnAudioSourceEnterHandler(const DeadFrame2D::Data::CollisionInfo& collisionInfo);

		void OnAudioSourceExitHandler(const DeadFrame2D::Data::CollisionInfo& collisionInfo);

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

		float GetVolume() const;
	};
}