#include "Core/Framerate/FrameTimer.h"
#include "Core/SubSystems/Systems/PhysicsEngine2D.h"
#include "Data/Collision/ContactListener.h"
#include "Factories/Concretions/Debugging/ColliderDrawerFactory.h"
#include "Utilities/Serialization/JsonSerializer.h"
#include <box2d/box2d.h>
#include <cassert>
#include <Constants/PhysicsConstants.h>
#include <Constants/ResourcePaths.h>


namespace DeadFrame2D::Core
{
	using namespace Shared::Constants;
	using namespace Shared::Models;

	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;
	using namespace DeadFrame2D::Factories;


	PhysicsEngine2D* PhysicsEngine2D::instance;


	PhysicsEngine2D::PhysicsEngine2D(const Vector2F& gravity)
	{
		assert(instance == nullptr && "PhysicsEngine2D was already initialized!");

		instance = this;

		contactListener = std::make_unique<ContactListener>();

		debugDrawer = std::unique_ptr<b2Draw>(ColliderDrawerFactory().CreateProduct());


		world = std::make_unique<b2World>(b2Vec2(gravity.x, gravity.y));

		world->SetContactListener(contactListener.get());

		world->SetDebugDraw(debugDrawer.get());


		collisionMasks = DeserializeFromFile<CollisionMasks>(Paths::Files::COLLISION_MASKS);


		std::cout << "[Info] SDL_image successfully initialized." << std::endl;
	}

	PhysicsEngine2D::~PhysicsEngine2D()
	{
		world.reset();

		instance = nullptr;

		std::cout << "[Info] PhysicsEngine2D subsystem successfully quit." << std::endl;
	}

	void PhysicsEngine2D::Update(float deltaTime)
	{

	}

	void PhysicsEngine2D::BeginFrame()
	{

	}

	void PhysicsEngine2D::EndUpdate()
	{
		world->Step(FrameTimer::DeltaTime(), Physics::VELOCITY_ITERATIONS, Physics::POSITION_ITERATIONS);
	}

	void PhysicsEngine2D::EndDraw()
	{
		world->DebugDraw();
	}

	Vector2F PhysicsEngine2D::GetGravity()
	{
		auto b2Gravity = instance->world->GetGravity();

		return Vector2F(b2Gravity.x, b2Gravity.y);
	}

	void PhysicsEngine2D::SetGravity(const Vector2F& newGravity)
	{
		auto b2Gravity = b2Vec2(newGravity.x, newGravity.y);

		instance->world->SetGravity(b2Gravity);
	}

	b2Body* PhysicsEngine2D::CreateBody(const b2BodyDef* bodyDef)
	{
		return instance->world->CreateBody(bodyDef);
	}

	void PhysicsEngine2D::DestroyBody(b2Body* bodyToDestroy)
	{
		return instance->world->DestroyBody(bodyToDestroy);
	}

	const CollisionMasks& PhysicsEngine2D::GetCollisionMasks()
	{
		return instance->collisionMasks;
	}
}