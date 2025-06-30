#include "Data/Collision/ContactListener.h"
#include "Factories/Concretions/Debugging/ColliderDrawerFactory.h"
#include "SubSystems/Physics/PhysicsEngine2D.h"
#include "Tools/FrameTimer.h"
#include <box2d/box2d.h>
#include <Constants/PhysicsConstants.h>


std::unique_ptr<b2World> PhysicsEngine2D::world;


PhysicsEngine2D::PhysicsEngine2D(const Vector2F& gravity)
{
	auto b2Gravity = b2Vec2(gravity.x, gravity.y);
	
	world = std::make_unique<b2World>(b2Gravity);

	world->SetContactListener(new ContactListener());

	world->SetDebugDraw(ColliderDrawerFactory().CreateProduct());

	std::cout << "[Info] SDL_image successfully initialized." << std::endl;
}

PhysicsEngine2D::~PhysicsEngine2D()
{
	world.reset();

	std::cout << "[Info] PhysicsEngine2D subsystem successfully quit." << std::endl;
}

void PhysicsEngine2D::BeginFrame()
{
}

void PhysicsEngine2D::EndUpdate()
{
	world->Step(FrameTimer::DeltaTime(), PhysicsConstants::VELOCITY_ITERATIONS, PhysicsConstants::POSITION_ITERATIONS);
}

void PhysicsEngine2D::EndDraw()
{
	world->DebugDraw();
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

b2Body* PhysicsEngine2D::CreateBody(const b2BodyDef* bodyDef)
{
	return world->CreateBody(bodyDef);
}

void PhysicsEngine2D::DestroyBody(b2Body* bodyToDestroy)
{
	return world->DestroyBody(bodyToDestroy);
}