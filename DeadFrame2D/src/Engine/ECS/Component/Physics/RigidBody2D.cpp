#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Context/Systems/Physics/PhysicsEngine2D.h"
#include "Core/Math/MathUtils.h"
#include "Engine/ECS/Component/Physics/RigidBody2D.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;
	using namespace DF2D::Constants;


	RigidBody2D::RigidBody2D(const BodyDefinition2D& bodyDefinition)
		: coroutineScheduler(nullptr),
		physicsEngine(nullptr),
		bodyDefinition(bodyDefinition),
		body(0),
		lastTransformPosition(Vector2F::Zero),
		lastTransformRotation(0.0f)
	{
		pendingActions.Clear();
	}

	RigidBody2D::~RigidBody2D()
	{
		if (physicsEngine == nullptr || body == 0)
			return;

		physicsEngine->DestroyBody(body);

		body = 0;
	}

	void RigidBody2D::OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool isActive)
	{
		coroutineScheduler->StartCoroutine(SetEnabled(isActive));
	}

	Task RigidBody2D::SetEnabled(bool isEnabled)
	{
		// Can't delete a fixture during a physics callback (e.g., contact begin)
		// because the world is locked. Defer until the next frame update.
		co_await CoroutineHelpers::WaitFrame();

		physicsEngine->SetBodyEnabled(body, isEnabled);
	}

	void RigidBody2D::Init()
	{
		coroutineScheduler = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().coroutineScheduler, NAME_OF(coroutineScheduler));
		physicsEngine = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().physicsEngine, NAME_OF(physicsEngine));
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));

		body = physicsEngine->CreateBody(bodyDefinition);

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();

		auto angleRad = MathUtils::ToRadians(lastTransformRotation);

		physicsEngine->SetBodyTransform(body, lastTransformPosition, angleRad);
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
		auto currentTransformPosition = transform->GetWorldPosition();
		auto currentTransformRotation = transform->GetWorldRotation();

		if (currentTransformPosition != lastTransformPosition || currentTransformRotation != lastTransformRotation)
		{
			auto bodyTransform = physicsEngine->GetBodyTransform(body);

			if (currentTransformPosition != lastTransformPosition)
				bodyTransform.position = currentTransformPosition;

			if (currentTransformRotation != lastTransformRotation)
				bodyTransform.angle = MathUtils::ToRadians(currentTransformRotation);

			physicsEngine->SetBodyTransform(body, bodyTransform.position, bodyTransform.angle);
		}

		auto bodyTransform = physicsEngine->GetBodyTransform(body);

		// Sync transform to physics body
		transform->SetWorldPosition(bodyTransform.position);
		transform->SetLocalRotation(MathUtils::ToDegrees(bodyTransform.angle));

		lastTransformPosition = transform->GetWorldPosition();
		lastTransformRotation = transform->GetWorldRotation();
	}

	FixtureID RigidBody2D::CreateFixture(const PhysicsMaterial& physicsMaterial, const ComponentHandle<ContactEventProvider>& contactEventProvider)
	{
		return physicsEngine->CreateFixture(body, physicsMaterial, contactEventProvider);
	}

	void RigidBody2D::ChangeBodyType(BodyType2D newBodyType)
	{
		pendingActions.AddLambda([this, newBodyType]()
			{
				physicsEngine->SetBodyType(body, newBodyType);
			});
	}

	void RigidBody2D::DestroyFixture(FixtureID fixture)
	{
		physicsEngine->DestroyFixture(fixture);
	}

	Vector2F RigidBody2D::GetVelocity() const
	{
		return physicsEngine->GetLinearVelocity(body);
	}

	void RigidBody2D::SetVelocity(const Vector2F& velocity)
	{
		physicsEngine->SetLinearVelocity(body, velocity);
	}

	void RigidBody2D::SetVelocityX(float velX)
	{
		auto currentVelocity = physicsEngine->GetLinearVelocity(body);

		physicsEngine->SetLinearVelocity(body, Vector2F(velX, currentVelocity.y));
	}

	void RigidBody2D::SetVelocityY(float velY)
	{
		auto currentVelocity = physicsEngine->GetLinearVelocity(body);

		physicsEngine->SetLinearVelocity(body, Vector2F(currentVelocity.x, velY));
	}

	void RigidBody2D::AddImpulse(const Vector2F& impulse)
	{
		physicsEngine->ApplyLinearImpulseToCenter(body, impulse);
	}

	void RigidBody2D::AddImpulseX(float impulseX)
	{
		physicsEngine->ApplyLinearImpulseToCenter(body, Vector2F(impulseX, 0.0f));
	}

	void RigidBody2D::AddImpulseY(float impulseY)
	{
		physicsEngine->ApplyLinearImpulseToCenter(body, Vector2F(0.0f, impulseY));
	}

	void RigidBody2D::AddForce(const Vector2F& force)
	{
		physicsEngine->ApplyForceToCenter(body, force);
	}

	void RigidBody2D::SetGravityScale(float newGravityScale)
	{
		physicsEngine->SetBodyGravityScale(body, newGravityScale);
	}
}