#pragma once
#include "Components/Abstractions/ContactEventProvider.h"


class Transform;
class b2Body;
class b2Fixture;


class AudioListener : public ContactEventProvider
{
	friend class ContactListener;


protected:
	Transform* transform = nullptr;

	// Using Box2D to detect audio source collisions with audio listeners
	b2Body* collisionBody;
	
	b2Fixture* collisionFixture;

	Vector2F lastTransformPosition;

	float lastTransformRotation;


	void RebuildFixture();


public:
	AudioListener();
	
	virtual ~AudioListener() override;


	virtual void Init() override;
	
	virtual void Start() override;
	
	virtual void Update(float deltaTime) override;

	virtual void Draw() override;
};