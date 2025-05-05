#include "Components/Physics/RigidBody2D.h"
#include "Components/Transform.h"
#include "GameObject.h"


RigidBody2D::RigidBody2D(float mass, float linearDamping, bool useGravity)
	: mass(mass), linearDamping(linearDamping), useGravity(useGravity)
{
}

void RigidBody2D::Init()
{
	transform = OwningObject->GetComponent<Transform>();
}

void RigidBody2D::Update(float dt)
{
	startFrameVelocity = velocity;

	ApplyForces(dt);

	velocity += acceleration * dt;
	velocity *= (1.0f - linearDamping);

	transform->Translate(velocity * dt);

	ClearForces();
}

void RigidBody2D::Draw()
{
}

void RigidBody2D::AddForce(const Vector2F& force)
{
	totalForce += force;
}

void RigidBody2D::AddImpulse(const Vector2F& impulse)
{
	velocity += impulse / mass;
}

void RigidBody2D::SetVelocity(const Vector2F& v)
{
	velocity = v;
}

void RigidBody2D::SetAcceleration(const Vector2F& a)
{
	acceleration = a;
}

void RigidBody2D::SetMass(float m)
{
	mass = m;
}

void RigidBody2D::SetGravityEnabled(bool enabled)
{
	useGravity = enabled;
}

void RigidBody2D::SetGravity(const Vector2F& g)
{
	gravity = g;
}

void RigidBody2D::SetLinearDamping(float damping)
{
	linearDamping = damping;
}

Vector2F RigidBody2D::GetVelocity() const
{
	return velocity;
}

Vector2F RigidBody2D::GetStartFrameVelocity() const
{
	return startFrameVelocity;
}

Vector2F RigidBody2D::GetAcceleration() const
{
	return acceleration;
}

float RigidBody2D::GetMass() const
{
	return mass;
}

bool RigidBody2D::IsUsingGravity() const
{
	return useGravity;
}

void RigidBody2D::ApplyForces(float dt)
{
	auto netForce = totalForce;

	if (useGravity)
	{
		netForce += gravity * mass;
	}

	acceleration = netForce / mass;
}

void RigidBody2D::ClearForces()
{
	totalForce = Vector2F::Zero;
}