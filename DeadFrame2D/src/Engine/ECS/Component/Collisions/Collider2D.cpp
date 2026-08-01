#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Engine/ECS/Component/Collisions/Collider2D.h"
#include "Engine/ECS/Component/Physics/RigidBody2D.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/Events/GameObject/GameObjectCreatedEvent.h"
#include "Engine/Events/GameObject/GameObjectDestroyedEvent.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"
#include <Utilities/Debugging/Guards.h>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	Collider2D::Collider2D(const PhysicsMaterial& physicsMaterial)
		: fixture(0),
		physicsMaterial(physicsMaterial)
	{
	}

	Collider2D::~Collider2D()
	{
		if (fixture <= 0 || rigidBody == nullptr)
			return;

		rigidBody->DestroyFixture(fixture);
	}

	void Collider2D::RebuildFixture()
	{
		if (fixture > 0 && rigidBody != nullptr)
		{
			rigidBody->DestroyFixture(fixture);

			fixture = 0;
		}

		SearchRigidBody();

		if (rigidBody == nullptr)
			return;

		fixture = rigidBody->CreateFixture(physicsMaterial, this);

		isDirty = false;
	}

	void Collider2D::SearchRigidBody()
	{
		if (rigidBody != nullptr)
			return;

		rigidBody = GetGameObject()->GetComponentInParent<RigidBody2D>(/*recursive*/ true, /*includeSelf*/ true);
	}

	void Collider2D::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));

		SearchRigidBody();

		MarkDirty();
	}

	void Collider2D::Update(float dt)
	{
		if (isDirty)
		{
			RebuildFixture();
		}
	}

	bool Collider2D::IsTrigger() const
	{
		return physicsMaterial.isSensor;
	}

	void Collider2D::SetIsTrigger(bool value)
	{
		physicsMaterial.isSensor = value;

		MarkDirty();
	}

	ComponentHandle<Transform> Collider2D::GetTranform() const
	{
		return transform;
	}

	const PhysicsMaterial& Collider2D::GetPhysicsMaterial()
	{
		return physicsMaterial;
	}
}