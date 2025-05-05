#pragma once
#include "Components/GameComponent.h"
#include "Constants/PhysicsConstants.h"
#include "Math/Vector2.h"


class Transform;


class RigidBody2D : public GameComponent
{
private:
	Transform* transform;

	mutable Vector2F velocity = Vector2F::Zero;
	
	mutable Vector2F acceleration = Vector2F::Zero;

	float mass = 1.0f;
	
	float linearDamping = 0.1f;
	
	bool useGravity = true;

	Vector2F gravity = Vector2F(PhysicsConstants::GRAVITY_X, PhysicsConstants::GRAVITY_Y);

	Vector2F totalForce = Vector2F::Zero;

	Vector2F startFrameVelocity = Vector2F::Zero;


protected:
	void ApplyForces(float dt);
	
	void ClearForces();


public:
	RigidBody2D(float mass = 1.0f, float linearDamping = 0.1f, bool useGravity = true);

	RigidBody2D(RigidBody2D&& other) = default;
	
	virtual ~RigidBody2D() override = default;


	void Init() override;
	
	void Update(float dt) override;

	void Draw() override;


	void AddForce(const Vector2F& force);
	
	void AddImpulse(const Vector2F& impulse);

	void SetVelocity(const Vector2F& v);
	
	void SetAcceleration(const Vector2F& a);

	void SetMass(float m);

	void SetGravityEnabled(bool enabled);

	void SetGravity(const Vector2F& g);

	void SetLinearDamping(float damping);

	Vector2F GetVelocity() const;
	
	Vector2F GetStartFrameVelocity() const;
	
	Vector2F GetAcceleration() const;
	
	float GetMass() const;
	
	bool IsUsingGravity() const;
};