#include "Converters/Physics/PhysicsConversions.h"
#include "Core/Framerate/FrameTimer.h"
#include "Core/SubSystems/Systems/PhysicsEngine2D.h"
#include "Data/Collision/PhysicsMaterial.h"
#include "Engine/Components/Audio/AudioListener.h"
#include "Engine/Components/Transform.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Converters;


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

			auto bodyDefBox2d = Physics::ToB2BodyDef(bodyDef);

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
			.shape = Physics::ToB2CircleShape(50.0001f),
			.isSensor = true,
			.filter = FilterData
			{
				.categoryBits = PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("AUDIO"),
				.maskBits = PhysicsEngine2D::GetCollisionMasks().GetMaskFlagByName("AUDIO")
			}
		};

		auto fixtureDef = Physics::ToB2FixtureDef(physicsMat, reinterpret_cast<uintptr_t>(this));

		collisionFixture = collisionBody->CreateFixture(&fixtureDef);

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();

		const auto METER_PER_PIXEL = DeadFrame2D::Core::PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

		auto angleRad = lastTransformRotation * (MathConstants::PI_f / 180.0f);

		collisionBody->SetTransform(b2Vec2(lastTransformPosition.x * METER_PER_PIXEL, lastTransformPosition.y * METER_PER_PIXEL), angleRad);

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

		currentTransformPosition *= DeadFrame2D::Core::PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

		auto angleRad = currentTransformRotation * (MathConstants::PI_f / 180.0f);

		collisionBody->SetTransform(b2Vec2(currentTransformPosition.x, currentTransformPosition.y), angleRad);
		collisionBody->SetAwake(true);
	}

	void AudioListener::Draw()
	{

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();
	}
}