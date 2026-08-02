#include "Core/Context/Systems/Physics/Debugging/PhysicsDebugDrawer.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Data/Components/Collision/CollisionInfo.h"
#include "Engine/ECS/Component/Collisions/Abstractions/ContactEventProvider.h"
#include "Utilities/Debugging/Guards.h"
#include <iostream>


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Models;
	using namespace DF2D::Utilities;


	PhysicsEngine2D::PhysicsEngine2D(const PhysicsConfig& physicsConfig, CollisionMasks collisionMasks, std::unique_ptr<IPhysicsBackend> backend)
		: backend(std::move(backend)),
		debugDrawer(std::make_unique<PhysicsDebugDrawer>()),
		physicsConfig(physicsConfig),
		collisionMasks(std::move(collisionMasks)),
		isDebugDrawEnabled(physicsConfig.debugDrawEnabled)
	{
		Guard::AgainstNull(this->backend.get(), NAME_OF(backend));

		this->backend->SetContactSink(this);

		std::cout << "[Info] PhysicsEngine2D successfully initialized." << std::endl;
	}

	PhysicsEngine2D::~PhysicsEngine2D()
	{
		backend->SetContactSink(nullptr);

		std::cout << "[Info] PhysicsEngine2D subsystem successfully quit." << std::endl;
	}

	void PhysicsEngine2D::BeginFrame()
	{

	}

	void PhysicsEngine2D::PreUpdate(float deltaTime)
	{

	}

	void PhysicsEngine2D::EndUpdate(float deltaTime)
	{
		backend->Step(deltaTime, physicsConfig.velocityIterations, physicsConfig.positionIterations);
	}

	void PhysicsEngine2D::EndDraw()
	{
		if (!isDebugDrawEnabled)
			return;

		backend->DebugDraw(*debugDrawer);

		debugDrawer->Flush();
	}

	void PhysicsEngine2D::OnContactBegin(FixtureID fixtureA, FixtureID fixtureB, const Vector2F& contactPoint, const Vector2F& normal)
	{
		auto recordA = fixtureRecords.find(fixtureA);
		auto recordB = fixtureRecords.find(fixtureB);

		if (recordA == fixtureRecords.end() || recordB == fixtureRecords.end())
			return;

		auto* providerA = recordA->second.provider();
		auto* providerB = recordB->second.provider();

		if (providerA == nullptr || providerB == nullptr)
			return;

		auto infoA = CollisionInfo
		{
			.contactPoint = contactPoint,
			.normal = normal,
			.thisGameObject = providerA->GetGameObject(),
			.otherGameObject = providerB->GetGameObject(),
		};

		auto infoB = CollisionInfo
		{
			.contactPoint = contactPoint,
			.normal = normal * -1,
			.thisGameObject = providerB->GetGameObject(),
			.otherGameObject = providerA->GetGameObject()
		};

		providerA->InvokeCollisionEnter(infoA);
		providerB->InvokeCollisionEnter(infoB);
	}

	void PhysicsEngine2D::OnContactEnd(FixtureID fixtureA, FixtureID fixtureB)
	{
		auto recordA = fixtureRecords.find(fixtureA);
		auto recordB = fixtureRecords.find(fixtureB);

		if (recordA == fixtureRecords.end() || recordB == fixtureRecords.end())
			return;

		auto* providerA = recordA->second.provider();
		auto* providerB = recordB->second.provider();

		if (providerA == nullptr || providerB == nullptr)
			return;

		auto infoA = CollisionInfo
		{
			.contactPoint = Vector2F(),
			.normal = Vector2F(),
			.thisGameObject = providerA->GetGameObject(),
			.otherGameObject = providerB->GetGameObject(),
		};

		auto infoB = CollisionInfo
		{
			.contactPoint = Vector2F(),
			.normal = Vector2F(),
			.thisGameObject = providerB->GetGameObject(),
			.otherGameObject = providerA->GetGameObject()
		};

		providerA->InvokeCollisionExit(infoA);
		providerB->InvokeCollisionExit(infoB);
	}

	Vector2F PhysicsEngine2D::GetGravity() const
	{
		return Vector2F(physicsConfig.gravityX, physicsConfig.gravityY);
	}

	void PhysicsEngine2D::SetGravity(const Vector2F& newGravity)
	{
		physicsConfig.gravityX = newGravity.x;
		physicsConfig.gravityY = newGravity.y;

		backend->SetGravity(newGravity);
	}

	BodyID PhysicsEngine2D::CreateBody(const BodyDefinition2D& bodyDefinition)
	{
		return backend->CreateBody(bodyDefinition);
	}

	void PhysicsEngine2D::DestroyBody(BodyID body)
	{
		if (body == 0)
			return;

		// Purge records first so contact-end events fired during destruction find no provider
		std::erase_if(fixtureRecords, [body](const auto& record)
			{
				return record.second.body == body;
			});

		backend->DestroyBody(body);
	}

	FixtureID PhysicsEngine2D::CreateFixture(BodyID body, const PhysicsMaterial& physicsMaterial, const ComponentHandle<ContactEventProvider>& contactEventProvider)
	{
		if (body == 0)
			return 0;

		auto fixture = backend->CreateFixture(body, physicsMaterial);

		if (fixture == 0)
			return 0;

		fixtureRecords[fixture] = FixtureRecord
		{
			.provider = contactEventProvider,
			.body = body
		};

		return fixture;
	}

	void PhysicsEngine2D::DestroyFixture(FixtureID fixture)
	{
		if (fixture == 0)
			return;

		// Purge the record first so contact-end events fired during destruction find no provider
		fixtureRecords.erase(fixture);

		backend->DestroyFixture(fixture);
	}

	void PhysicsEngine2D::SetBodyEnabled(BodyID body, bool isEnabled)
	{
		backend->SetBodyEnabled(body, isEnabled);
	}

	void PhysicsEngine2D::SetBodyType(BodyID body, BodyType2D newBodyType)
	{
		backend->SetBodyType(body, newBodyType);
	}

	void PhysicsEngine2D::SetBodyTransform(BodyID body, const Vector2F& position, float angle)
	{
		backend->SetBodyTransform(body, position, angle);
	}

	BodyTransform2D PhysicsEngine2D::GetBodyTransform(BodyID body) const
	{
		return backend->GetBodyTransform(body);
	}

	void PhysicsEngine2D::SetBodyAwake(BodyID body, bool isAwake)
	{
		backend->SetBodyAwake(body, isAwake);
	}

	void PhysicsEngine2D::SetBodyGravityScale(BodyID body, float gravityScale)
	{
		backend->SetBodyGravityScale(body, gravityScale);
	}

	Vector2F PhysicsEngine2D::GetLinearVelocity(BodyID body) const
	{
		return backend->GetLinearVelocity(body);
	}

	void PhysicsEngine2D::SetLinearVelocity(BodyID body, const Vector2F& velocity)
	{
		backend->SetLinearVelocity(body, velocity);
	}

	void PhysicsEngine2D::ApplyLinearImpulseToCenter(BodyID body, const Vector2F& impulse)
	{
		backend->ApplyLinearImpulseToCenter(body, impulse);
	}

	void PhysicsEngine2D::ApplyForceToCenter(BodyID body, const Vector2F& force)
	{
		backend->ApplyForceToCenter(body, force);
	}

	const PhysicsConfig& PhysicsEngine2D::GetPhysicsConfig() const
	{
		return physicsConfig;
	}

	const CollisionMasks& PhysicsEngine2D::GetCollisionMasks() const
	{
		return collisionMasks;
	}

	void PhysicsEngine2D::SetDebugDrawEnabled(bool isEnabled)
	{
		isDebugDrawEnabled = isEnabled;
	}
}