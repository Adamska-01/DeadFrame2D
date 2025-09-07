#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Physics/BodyDefinition2D.h"
#include "Data/Physics/BodyType2D.h"
#include "Engine/Components/GameComponent.h"
#include "Utilities/Delegates/MulticastDelegate.h"


struct b2FixtureDef;
class b2Fixture;
class b2Body;
class Transform;


class RigidBody2D : public GameComponent
{
private:
	Transform* transform;

	b2Body* body;

	mutable Vector2F velocity;
	
	mutable Vector2F acceleration;

	MultiCastVoid pendingActions;

	Vector2F lastTransformPosition;

	float lastTransformRotation;

	
public:
	RigidBody2D(const BodyDefinition2D& bodyDefinition);

	RigidBody2D(RigidBody2D&& other) = default;
	
	virtual ~RigidBody2D() override;


	void Init() override;

	void Start() override;
	
	void Update(float deltaTime) override;

	void Draw() override;


	b2Fixture* CreateFixture(const b2FixtureDef* fixtureDef);

	void ChangeBodyType(BodyType2D newBodyType);

	void DestroyFixture(b2Fixture* fixtureDef);


	Vector2F GetVelocity() const;

	void SetVelocity(const Vector2F& velocity);

	void SetVelocityX(float velX);
	
	void SetVelocityY(float velY);

	void AddImpulse(const Vector2F& impulse);

	void AddImpulseX(float impulseX);
	
	void AddImpulseY(float impulseY);

	void AddForce(const Vector2F& force);

	void SetGravityScale(float newGravityScale);
};