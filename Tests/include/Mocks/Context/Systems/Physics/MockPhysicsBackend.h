#pragma once
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsBackend.h"
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsContactSink.h"
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsDebugDraw.h"
#include <vector>


struct MockPhysicsBackend : DF2D::Core::IPhysicsBackend
{
	// Configurable results
	DF2D::Data::BodyID nextBodyId{1};

	DF2D::Data::FixtureID nextFixtureId{1};

	DF2D::Data::BodyTransform2D bodyTransformResult{};

	DF2D::Core::Vector2F linearVelocityResult{};

	bool createBodyFails{false};

	bool createFixtureFails{false};

	// Emulates Box2D firing an end-contact event while a fixture is being destroyed
	bool destroyFixtureFiresEndContact{false};

	DF2D::Data::FixtureID endContactOtherFixture{0};


	// Call tracking
	DF2D::Core::IPhysicsContactSink* sink{nullptr};

	int stepCount{0};

	float lastStepDeltaTime{0.0f};

	int lastVelocityIterations{0};

	int lastPositionIterations{0};

	DF2D::Core::Vector2F lastGravity{};

	int setGravityCount{0};

	std::vector<DF2D::Data::BodyID> createdBodies;

	std::vector<DF2D::Data::BodyID> destroyedBodies;

	DF2D::Data::BodyDefinition2D lastBodyDefinition{};

	std::vector<DF2D::Data::FixtureID> createdFixtures;

	std::vector<DF2D::Data::FixtureID> destroyedFixtures;

	DF2D::Data::BodyID lastFixtureBody{0};

	DF2D::Data::PhysicsMaterial lastPhysicsMaterial{};

	DF2D::Data::BodyID lastBodyOpTarget{0};

	bool lastBodyEnabled{true};

	DF2D::Data::BodyType2D lastBodyType{DF2D::Data::BodyType2D::Static};

	DF2D::Core::Vector2F lastBodyPosition{};

	float lastBodyAngle{0.0f};

	bool lastBodyAwake{false};

	float lastGravityScale{1.0f};

	DF2D::Core::Vector2F lastLinearVelocity{};

	DF2D::Core::Vector2F lastImpulse{};

	DF2D::Core::Vector2F lastForce{};

	int debugDrawCount{0};

	DF2D::Core::IPhysicsDebugDraw* lastDebugDrawTarget{nullptr};


	void Step(float deltaTime, int velocityIterations, int positionIterations) override
	{
		stepCount++;
		lastStepDeltaTime = deltaTime;
		lastVelocityIterations = velocityIterations;
		lastPositionIterations = positionIterations;
	}

	void SetGravity(const DF2D::Core::Vector2F& gravity) override
	{
		setGravityCount++;
		lastGravity = gravity;
	}

	DF2D::Data::BodyID CreateBody(const DF2D::Data::BodyDefinition2D& bodyDefinition) override
	{
		lastBodyDefinition = bodyDefinition;

		if (createBodyFails)
			return 0;

		auto body = nextBodyId++;

		createdBodies.push_back(body);

		return body;
	}

	void DestroyBody(DF2D::Data::BodyID body) override
	{
		destroyedBodies.push_back(body);
	}

	DF2D::Data::FixtureID CreateFixture(DF2D::Data::BodyID body, const DF2D::Data::PhysicsMaterial& physicsMaterial) override
	{
		lastFixtureBody = body;
		lastPhysicsMaterial = physicsMaterial;

		if (createFixtureFails)
			return 0;

		auto fixture = nextFixtureId++;

		createdFixtures.push_back(fixture);

		return fixture;
	}

	void DestroyFixture(DF2D::Data::FixtureID fixture) override
	{
		destroyedFixtures.push_back(fixture);

		if (destroyFixtureFiresEndContact && sink != nullptr)
			sink->OnContactEnd(fixture, endContactOtherFixture);
	}

	void SetBodyEnabled(DF2D::Data::BodyID body, bool isEnabled) override
	{
		lastBodyOpTarget = body;
		lastBodyEnabled = isEnabled;
	}

	void SetBodyType(DF2D::Data::BodyID body, DF2D::Data::BodyType2D newBodyType) override
	{
		lastBodyOpTarget = body;
		lastBodyType = newBodyType;
	}

	void SetBodyTransform(DF2D::Data::BodyID body, const DF2D::Core::Vector2F& position, float angle) override
	{
		lastBodyOpTarget = body;
		lastBodyPosition = position;
		lastBodyAngle = angle;
	}

	DF2D::Data::BodyTransform2D GetBodyTransform(DF2D::Data::BodyID body) override
	{
		lastBodyOpTarget = body;

		return bodyTransformResult;
	}

	void SetBodyAwake(DF2D::Data::BodyID body, bool isAwake) override
	{
		lastBodyOpTarget = body;
		lastBodyAwake = isAwake;
	}

	void SetBodyGravityScale(DF2D::Data::BodyID body, float gravityScale) override
	{
		lastBodyOpTarget = body;
		lastGravityScale = gravityScale;
	}

	DF2D::Core::Vector2F GetLinearVelocity(DF2D::Data::BodyID body) override
	{
		lastBodyOpTarget = body;

		return linearVelocityResult;
	}

	void SetLinearVelocity(DF2D::Data::BodyID body, const DF2D::Core::Vector2F& velocity) override
	{
		lastBodyOpTarget = body;
		lastLinearVelocity = velocity;
	}

	void ApplyLinearImpulseToCenter(DF2D::Data::BodyID body, const DF2D::Core::Vector2F& impulse) override
	{
		lastBodyOpTarget = body;
		lastImpulse = impulse;
	}

	void ApplyForceToCenter(DF2D::Data::BodyID body, const DF2D::Core::Vector2F& force) override
	{
		lastBodyOpTarget = body;
		lastForce = force;
	}

	void SetContactSink(DF2D::Core::IPhysicsContactSink* newSink) override
	{
		sink = newSink;
	}

	void DebugDraw(DF2D::Core::IPhysicsDebugDraw& drawer) override
	{
		debugDrawCount++;
		lastDebugDrawTarget = &drawer;
	}
};