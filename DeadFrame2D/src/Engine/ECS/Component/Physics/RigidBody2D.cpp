#include "Converters/Physics/PhysicsConversions.h"
#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Data/Components/Physics/BodyDefinition2D.h"
#include "Engine/ECS/Component/Physics/RigidBody2D.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"
#include <box2d/b2_body.h>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Internal;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;
	using namespace DF2D::Constants;


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

	void RigidBody2D::OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool isActive)
	{
		CoroutineScheduler::StartCoroutine(SetEnabled(isActive));
	}

	Task RigidBody2D::SetEnabled(bool isEnabled)
	{
		// Can't delete a fixture during a Box2D callback (e.g., BeginContact)
		// because the world is locked. Defer deletion until the next frame update.
		co_await WaitFrame();

		body->SetEnabled(isEnabled);
	}

	void RigidBody2D::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();

		auto METER_PER_PIXEL = PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

		auto angleRad = lastTransformRotation * (MathConstants::PI_f / 180.0f);

		body->SetTransform(b2Vec2(lastTransformPosition.x * METER_PER_PIXEL, lastTransformPosition.y * METER_PER_PIXEL), angleRad);
	}

	void RigidBody2D::Update(float deltaTime)
	{
		if (!pendingActions.IsEmpty())
		{
			pendingActions.Broadcast();

			pendingActions.Clear();
		}
	}

	void RigidBody2D::LateUpdate(float deltaTime)
	{
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

	b2Fixture* RigidBody2D::CreateFixture(const b2FixtureDef* fixtureDef)
	{
		return body->CreateFixture(fixtureDef);
	}

	void RigidBody2D::ChangeBodyType(BodyType2D newBodyType)
	{
		pendingActions.AddLambda([this, newBodyType]()
			{
				body->SetType(Physics::ToB2BodyType(newBodyType));
			});
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