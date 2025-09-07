#include "Core/Framerate/FrameTimer.h"
#include "Core/SubSystems/Systems/PhysicsEngine2D.h"
#include "Data/Collision/PhysicsMaterial.h"
#include "Engine/Components/Audio/AudioListener.h"
#include "Engine/Components/Transform.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Helpers/Physics/PhysicsConversion.h"
#include "Utilities/Helpers/Physics/PhysicsShapeCreators.h"


using namespace DeadFrame2D::Constants;


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
		.isSensor = true,
		.filter = FilterData
		{
			.categoryBits = PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("AUDIO"),
			.maskBits = PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("AUDIO")
		}
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

	auto currentTransformPosition = transform->GetWorldPosition();
	auto currentTransformRotation = transform->GetWorldRotation();

	if (currentTransformPosition == lastTransformPosition && currentTransformRotation == currentTransformRotation)
		return;

	currentTransformPosition *= PhysicsConstants::PIXEL_TO_METER;

	collisionBody->SetTransform(b2Vec2(currentTransformPosition.x, currentTransformPosition.y), currentTransformRotation * (MathConstants::PI / 180.0f));
	collisionBody->SetAwake(true);
}

void AudioListener::Draw()
{

	lastTransformPosition = transform->GetWorldPosition();
	lastTransformRotation = transform->GetWorldRotation();
}