#include "SubSystems/Physics/PhysicsEngine2D.h"
#include "Tools/FrameTimer.h"
#include <box2d/box2d.h>
#include <Constants/PhysicsConstants.h>


std::unique_ptr<b2World> PhysicsEngine2D::world;


PhysicsEngine2D::PhysicsEngine2D(const Vector2F& gravity)
	: velocityIterations(PhysicsConstants::VELOCITY_ITERATIONS),
	positionIterations(PhysicsConstants::POSITION_ITERATIONS)
{
	auto b2Gravity = b2Vec2(gravity.x, gravity.y);

	world = std::make_unique<b2World>(b2Gravity);
}

void PhysicsEngine2D::BeginFrame()
{
	world->Step(FrameTimer::DeltaTime(), velocityIterations, positionIterations);
}

void PhysicsEngine2D::SetContactListener(b2ContactListener* listener)
{
	world->SetContactListener(listener);
}

Vector2F PhysicsEngine2D::GetGravity()
{
	auto b2Gravity = world->GetGravity();

	return Vector2F(b2Gravity.x, b2Gravity.y);
}

void PhysicsEngine2D::SetGravity(const Vector2F& newGravity)
{
	auto b2Gravity = b2Vec2(newGravity.x, newGravity.y);

	world->SetGravity(b2Gravity);
}