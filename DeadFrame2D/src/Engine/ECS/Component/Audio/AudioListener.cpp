#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Core/Math/MathUtils.h"
#include "Data/Components/Collision/PhysicsMaterial.h"
#include "Engine/ECS/Component/Audio/AudioListener.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Constants;
	using namespace DF2D::Utilities;


	AudioListener::AudioListener()
		: physicsEngine(nullptr),
		collisionBody(0),
		collisionFixture(0),
		lastTransformPosition(Vector2F::Zero),
		lastTransformRotation(0.0f)
	{
	}

	AudioListener::~AudioListener()
	{
		if (physicsEngine == nullptr || collisionBody == 0)
			return;

		physicsEngine->DestroyBody(collisionBody);
		collisionBody = 0;
	}

	void AudioListener::RebuildFixture()
	{
		if (collisionBody == 0)
		{
			auto bodyDef = BodyDefinition2D
			{
				.type = BodyType2D::Kinematic,
				.gravityScale = 0.0f
			};

			collisionBody = physicsEngine->CreateBody(bodyDef);
		}

		if (collisionFixture != 0)
		{
			physicsEngine->DestroyFixture(collisionFixture);

			collisionFixture = 0;
		}

		auto physicsMat = PhysicsMaterial
		{
			// Tiny circle (box2d doesn't support dots)
			.shape = CircleShapeDefinition2D
			{
				.radius = 50.0001f
			},
			.isSensor = true,
			.filter = FilterData
			{
				.categoryBits = physicsEngine->GetCollisionMasks().GetMaskFlagByName("AUDIO"),
				.maskBits = physicsEngine->GetCollisionMasks().GetMaskFlagByName("AUDIO")
			}
		};

		collisionFixture = physicsEngine->CreateFixture(collisionBody, physicsMat, GetHandleAs<ContactEventProvider>());

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();

		auto angleRad = MathUtils::ToRadians(lastTransformRotation);

		physicsEngine->SetBodyTransform(collisionBody, lastTransformPosition, angleRad);

		isDirty = false;
	}

	void AudioListener::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
		physicsEngine = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().physicsEngine, NAME_OF(physicsEngine));

		isDirty = true;
	}

	void AudioListener::Update(float deltaTime)
	{
		if (isDirty)
		{
			RebuildFixture();
		}
	}

	void AudioListener::LateUpdate(float deltaTime)
	{
		auto currentTransformPosition = transform->GetWorldPosition();
		auto currentTransformRotation = transform->GetWorldRotation();

		if (currentTransformPosition == lastTransformPosition
			&& currentTransformRotation == lastTransformRotation)
			return;

		auto angleRad = MathUtils::ToRadians(currentTransformRotation);

		physicsEngine->SetBodyTransform(collisionBody, currentTransformPosition, angleRad);
		physicsEngine->SetBodyAwake(collisionBody, true);
	}

	void AudioListener::Draw()
	{

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();
	}
}