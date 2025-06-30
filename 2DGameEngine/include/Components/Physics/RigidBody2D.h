#pragma once
#include "Components/GameComponent.h"
#include "Data/Physics/BodyDefinition2D.h"
#include "Data/Physics/BodyType2D.h"
#include "Math/Vector2.h"
#include "Tools/MulticastDelegate.h"


struct b2FixtureDef;
class b2Fixture;
class b2Body;
class Transform;


class RigidBody2D : public GameComponent
{
private:
	Transform* transform;

	b2Body* body;

	mutable Vector2F velocity = Vector2F::Zero;
	
	mutable Vector2F acceleration = Vector2F::Zero;

	MultiCastVoid pendingActions;

	Vector2F lastTransformPosition;

	float lastTransformRotation;

	
public:
	RigidBody2D(const BodyDefinition2D& bodyDefinition, float gravityScale = 1.0f);

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

	void AddForce(const Vector2F& force);
	
	void AddLinearImpulse(const Vector2F& impulse);

	void SetGravityScale(float newGravityScale);
};