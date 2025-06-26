#include "Components/Audio/AudioListener.h"
#include "Components/Transform.h"
#include "Data/Collision/PhysicsMaterial.h"
#include "GameObject.h"
#include "SubSystems/Physics/PhysicsEngine2D.h"
#include "Tools/Helpers/Physics/PhysicsConversion.h"
#include "Tools/Helpers/Physics/PhysicsShapeCreators.h"
#include <Tools/FrameTimer.h>


AudioListener::AudioListener()
	: transform(nullptr),
	collisionBody(nullptr),
	collisionFixture(nullptr),
	lastTransformPosition(Vector2F::Zero),
	lastTransformRotation(0.0f)
{
}

AudioListener::~AudioListener()
{
	if (collisionBody == nullptr)
		return;

	auto fixture = collisionBody->GetFixtureList();

	while (fixture != nullptr)
	{
		auto next = fixture->GetNext();

		collisionBody->DestroyFixture(fixture);

		fixture = next;
	}
}

void AudioListener::RebuildFixture()
{
	if (collisionBody == nullptr)
	{
		auto bodyDef = BodyDefinition2D
		{
			.type = BodyType2D::Dynamic,
			.gravityScale = 0.0f
		};

		auto bodyDefBox2d = PhysicsConversion::ToB2BodyDef(bodyDef);

		collisionBody = PhysicsEngine2D::CreateBody(&bodyDefBox2d);
	}

	if (collisionFixture != nullptr)
	{
		collisionBody->DestroyFixture(collisionFixture);

		collisionFixture = nullptr;
	}

	auto physicsMat = PhysicsMaterial
	{
		// Tiny circle (box2d doesn't support dots)
		.shape = PhysicsShapeCreators::CreateCircleShape(50.0001f),
		.isSensor = true
	};

	auto fixtureDef = PhysicsConversion::ToB2FixtureDef(physicsMat, reinterpret_cast<uintptr_t>(this));

	collisionFixture = collisionBody->CreateFixture(&fixtureDef);

	lastTransformPosition = transform->GetWorldPosition();
	lastTransformRotation = transform->GetWorldRotation();

	collisionBody->SetTransform(b2Vec2(lastTransformPosition.x * PhysicsConstants::PIXEL_TO_METER, lastTransformPosition.y * PhysicsConstants::PIXEL_TO_METER), lastTransformRotation * (MathConstants::PI / 180.0f));

	isDirty = false;
}

void AudioListener::Init()
{
	transform = OwningObject.lock()->GetTransform();

	isDirty = true;
}

void AudioListener::Start()
{
}

void AudioListener::Update(float deltaTime)
{
	if (isDirty)
	{
		RebuildFixture();
	}
}

void AudioListener::Draw()
{
	// TODO: Use LateUpdate for this!!!
	auto safeDelta = std::max(FrameTimer::DeltaTime(), std::numeric_limits<float>::epsilon());

	auto currentTransformPosition = transform->GetWorldPosition();
	auto posNow = b2Vec2(
		currentTransformPosition.x * PhysicsConstants::PIXEL_TO_METER,
		currentTransformPosition.y * PhysicsConstants::PIXEL_TO_METER);

	auto posLast = b2Vec2(
		lastTransformPosition.x * PhysicsConstants::PIXEL_TO_METER,
		lastTransformPosition.y * PhysicsConstants::PIXEL_TO_METER);

	auto velocity = posNow - posLast;
	velocity.x /= safeDelta;
	velocity.y /= safeDelta;

	auto currentRotationRadians = transform->GetWorldRotation() * (MathConstants::PI / 180.0f);
	auto lastRotationRadians = lastTransformRotation * (MathConstants::PI / 180.0f);
	auto angularVelocity = (currentRotationRadians - lastRotationRadians) / safeDelta;

	collisionBody->SetLinearVelocity(velocity);
	collisionBody->SetAngularVelocity(angularVelocity);

	lastTransformPosition = transform->GetWorldPosition();
	lastTransformRotation = transform->GetWorldRotation();
}