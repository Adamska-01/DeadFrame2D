#include "Core/Framerate/FrameTimer.h"
#include "Core/SubSystems/Systems/Physics/ContactListener.h"
#include "Core/SubSystems/Systems/Physics/PhysicsEngine2D.h"
#include "Factories/Concretions/Debugging/ColliderDrawerFactory.h"
#include <box2d/box2d.h>
#include <cassert>
#include <Constants/ResourcePaths.h>
#include <Tools/Serialization/JsonSerializer.h>


namespace DeadFrame2D::Core
{
	using namespace Shared::Constants;
	using namespace Shared::Models;
	using namespace Shared::Tools;

	using namespace DeadFrame2D::Factories;


	PhysicsEngine2D* PhysicsEngine2D::instance;


	PhysicsEngine2D::PhysicsEngine2D(const PhysicsConfig& physicsConfig)
		: physicsConfig(physicsConfig)
	{
		assert(instance == nullptr && "PhysicsEngine2D was already initialized!");

		instance = this;

		contactListener = std::make_unique<ContactListener>();

		debugDrawer = std::unique_ptr<b2Draw>(ColliderDrawerFactory().CreateProduct());


		world = std::make_unique<b2World>(b2Vec2(physicsConfig.gravityX, physicsConfig.gravityY));

		world->SetContactListener(contactListener.get());

		world->SetDebugDraw(debugDrawer.get());


		collisionMasks = DeserializeFromFile<CollisionMasks>(Paths::Files::COLLISION_MASKS);


		std::cout << "[Info] PhysicsEngine2D successfully initialized." << std::endl;
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
		world->Step(FrameTimer::DeltaTime(), physicsConfig.velocityIterations, physicsConfig.positionIterations);
	}

	void PhysicsEngine2D::EndDraw()
	{
		world->DebugDraw();
	}

	Vector2F PhysicsEngine2D::GetGravity()
	{
		return Vector2F(instance->physicsConfig.gravityX, instance->physicsConfig.gravityY);
	}

	void PhysicsEngine2D::SetGravity(const Vector2F& newGravity)
	{
		instance->physicsConfig.gravityX = newGravity.x;
		instance->physicsConfig.gravityY = newGravity.y;

		instance->world->SetGravity(b2Vec2(newGravity.x, newGravity.y));
	}

	b2Body* PhysicsEngine2D::CreateBody(const b2BodyDef* bodyDef)
	{
		return instance->world->CreateBody(bodyDef);
	}

	void PhysicsEngine2D::DestroyBody(b2Body* bodyToDestroy)
	{
		return instance->world->DestroyBody(bodyToDestroy);
	}

	const Shared::Models::PhysicsConfig& PhysicsEngine2D::GetPhysicsConfig()
	{
		return instance->physicsConfig;
	}

	const CollisionMasks& PhysicsEngine2D::GetCollisionMasks()
	{
		return instance->collisionMasks;
	}
}