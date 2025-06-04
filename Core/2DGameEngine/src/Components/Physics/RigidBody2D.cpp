#include "Components/Physics/RigidBody2D.h"
#include "Components/Transform.h"
#include "Constants/PhysicsConstants.h"
#include "Data/Physics/BodyDefinition2D.h"
#include "GameObject.h"
#include "SubSystems/Physics/PhysicsEngine2D.h"
#include "Tools/Helpers/Physics/PhysicsConversion.h"
#include <box2d/b2_body.h>


RigidBody2D::RigidBody2D(const BodyDefinition2D& bodyDefinition, float gravityScale)
{
	auto bodyDef = PhysicsConversion::ToB2BodyDef(bodyDefinition);

	body = PhysicsEngine2D::CreateBody(&bodyDef);

	SetGravityScale(gravityScale);

	pendingActions.Clear();

	lastTransformPosition = Vector2F::Zero;
	lastTransformRotation = 0.0f;
}

RigidBody2D::~RigidBody2D()
{
	if (body == nullptr)
		return;

	auto fixture = body->GetFixtureList();

	while (fixture != nullptr)
	{
		auto next = fixture->GetNext();

		body->DestroyFixture(fixture);
		
		fixture = next;
	}
}

void RigidBody2D::Init()
{
	transform = OwningObject.lock()->GetTransform();
	
	auto worldPos = transform->GetWorldPosition() * PhysicsConstants::PIXEL_TO_METER;

	body->SetTransform(b2Vec2(worldPos.x, worldPos.y), 0);
}

void RigidBody2D::Start()
{

}

void RigidBody2D::Update(float deltaTime)
{
	if (!pendingActions.IsEmpty())
	{
		pendingActions();

		pendingActions.Clear();
	}

	auto currentTransformPosition = transform->GetWorldPosition();
	auto currentTransformRotation = transform->GetWorldRotation();

	if (currentTransformPosition != lastTransformPosition)
	{
		currentTransformPosition = currentTransformPosition * PhysicsConstants::PIXEL_TO_METER;
		body->SetTransform(b2Vec2(currentTransformPosition.x, currentTransformPosition.y), body->GetAngle());
	}
	if (currentTransformRotation != lastTransformRotation)
	{
		body->SetTransform(body->GetPosition(), currentTransformRotation * (MathConstants::PI / 180.0f));
	}

	auto pos = body->GetPosition();
	auto angle = body->GetAngle();

	pos.x *= PhysicsConstants::METER_TO_PIXEL;
	pos.y *= PhysicsConstants::METER_TO_PIXEL;

	// Sync transform to physics body
	transform->SetWorldPosition(Vector2F(pos.x, pos.y));
	transform->SetLocalRotation(angle * (180.0f / MathConstants::PI));
}

void RigidBody2D::Draw()
{
	lastTransformPosition = transform->GetWorldPosition();
	lastTransformRotation = transform->GetWorldRotation();
}

b2Fixture* RigidBody2D::CreateFixture(const b2FixtureDef* fixtureDef)
{
	return body->CreateFixture(fixtureDef);
}

void RigidBody2D::ChangeBodyType(BodyType2D newBodyType)
{
	pendingActions.RegisterCallback([this, newBodyType]()
		{
			body->SetType(PhysicsConversion::ToB2BodyType(newBodyType));
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

void RigidBody2D::AddForce(const Vector2F& force)
{
	body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void RigidBody2D::AddLinearImpulse(const Vector2F& impulse)
{
	body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
}

void RigidBody2D::SetGravityScale(float newGravityScale)
{
	body->SetGravityScale(newGravityScale);
}