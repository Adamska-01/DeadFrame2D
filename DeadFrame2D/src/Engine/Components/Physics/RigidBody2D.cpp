#include "Converters/Physics/PhysicsConversions.h"
#include "Core/SubSystems/Systems/Physics/PhysicsEngine2D.h"
#include "Data/Physics/BodyDefinition2D.h"
#include "Engine/Components/Physics/RigidBody2D.h"
#include "Engine/Components/Transform.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include <box2d/b2_body.h>


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Converters;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;
	using namespace DeadFrame2D::Constants;


	RigidBody2D::RigidBody2D(const BodyDefinition2D& bodyDefinition)
		: velocity(Vector2F::Zero),
		acceleration(Vector2F::Zero),
		lastTransformPosition(Vector2F::Zero),
		lastTransformRotation(0.0f)
	{
		pendingActions.Clear();

		auto bodyDef = Physics::ToB2BodyDef(bodyDefinition);

		body = PhysicsEngine2D::CreateBody(&bodyDef);
	}

	RigidBody2D::~RigidBody2D()
	{
		if (body == nullptr)
			return;

		auto fixture = body->GetFixtureList();

		while (fixture != nullptr)
		{
			auto next = fixture->GetNext();

			// Set the user data to nullptr in case the destruction triggers an end contact event
			fixture->GetUserData().pointer = 0;

			body->DestroyFixture(fixture);

			fixture = next;
		}

		PhysicsEngine2D::DestroyBody(body);
	}

	void RigidBody2D::Init()
	{
		transform = Guard::AgainstNullAssignment(OwningObject.lock()->GetTransform(), NAME_OF(transform));

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();

		auto METER_PER_PIXEL = PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

		auto angleRad = lastTransformRotation * (MathConstants::PI_f / 180.0f);

		body->SetTransform(b2Vec2(lastTransformPosition.x * METER_PER_PIXEL, lastTransformPosition.y * METER_PER_PIXEL), angleRad);
	}

	void RigidBody2D::Start()
	{

	}

	void RigidBody2D::Update(float deltaTime)
	{
		// TODO: Move this stuff in the LateUpdate!!!! (Leaving it here could break transform movement)
		// Edit: Fucking do this already!!!!!
		if (!pendingActions.IsEmpty())
		{
			pendingActions();

			pendingActions.Clear();
		}

		auto PIXEL_PER_METER = PhysicsEngine2D::GetPhysicsConfig().pixelPerMeter;
		auto METER_PER_PIXEL = PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

		auto currentTransformPosition = transform->GetWorldPosition();
		auto currentTransformRotation = transform->GetWorldRotation();

		if (currentTransformPosition != lastTransformPosition)
		{
			currentTransformPosition = currentTransformPosition * METER_PER_PIXEL;
			body->SetTransform(b2Vec2(currentTransformPosition.x, currentTransformPosition.y), body->GetAngle());
		}
		if (currentTransformRotation != lastTransformRotation)
		{
			auto angleRad = currentTransformRotation * (MathConstants::PI_f / 180.0f);

			body->SetTransform(body->GetPosition(), angleRad);
		}

		auto pos = body->GetPosition();
		auto angle = body->GetAngle() * (180.0f / MathConstants::PI_f);

		pos.x *= PIXEL_PER_METER;
		pos.y *= PIXEL_PER_METER;

		// Sync transform to physics body
		transform->SetWorldPosition(Vector2F(pos.x, pos.y));
		transform->SetLocalRotation(angle);

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();
	}

	void RigidBody2D::Draw()
	{
	
	}

	b2Fixture* RigidBody2D::CreateFixture(const b2FixtureDef* fixtureDef)
	{
		return body->CreateFixture(fixtureDef);
	}

	void RigidBody2D::ChangeBodyType(BodyType2D newBodyType)
	{
		pendingActions.RegisterCallback([this, newBodyType]()
			{
				body->SetType(Physics::ToB2BodyType(newBodyType));
			},
			reinterpret_cast<uintptr_t>(this));
	}

	void RigidBody2D::DestroyFixture(b2Fixture* fixtureDef)
	{
		body->DestroyFixture(fixtureDef);
	}

	Vector2F RigidBody2D::GetVelocity() const
	{
		auto v = body->GetLinearVelocity();

		return Vector2F(v.x, v.y);
	}

	void RigidBody2D::SetVelocity(const Vector2F& velocity)
	{
		body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	}

	void RigidBody2D::SetVelocityX(float velX)
	{
		const auto& currentVelocity = body->GetLinearVelocity();

		body->SetLinearVelocity(b2Vec2(velX, currentVelocity.y));
	}

	void RigidBody2D::SetVelocityY(float velY)
	{
		const auto& currentVelocity = body->GetLinearVelocity();

		body->SetLinearVelocity(b2Vec2(currentVelocity.x, velY));
	}

	void RigidBody2D::AddImpulse(const Vector2F& impulse)
	{
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
	}

	void RigidBody2D::AddImpulseX(float impulseX)
	{
		body->ApplyLinearImpulseToCenter(b2Vec2(impulseX, 0.0f), true);
	}

	void RigidBody2D::AddImpulseY(float impulseY)
	{
		body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, impulseY), true);
	}

	void RigidBody2D::AddForce(const Vector2F& force)
	{
		body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
	}

	void RigidBody2D::SetGravityScale(float newGravityScale)
	{
		body->SetGravityScale(newGravityScale);
	}
}