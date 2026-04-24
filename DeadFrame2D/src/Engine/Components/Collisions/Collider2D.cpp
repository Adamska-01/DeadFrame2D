#include "Converters/Physics/PhysicsConversions.h"
#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Engine/Components/Collisions/Collider2D.h"
#include "Engine/Components/Physics/RigidBody2D.h"
#include "Engine/Components/Transform.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"
#include <Utilities/Debugging/Guards.h>


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Internal;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	Collider2D::Collider2D(const PhysicsMaterial& physicsMaterial)
		: fixture(nullptr), 
		physicsMaterial(physicsMaterial)
	{
	}

	Collider2D::~Collider2D()
	{
		if (fixture == nullptr || rigidBody == nullptr)
			return;

		// Set the user data to nullptr in case the destruction triggers an end contact event
		fixture->GetUserData().pointer = 0;

		rigidBody->DestroyFixture(fixture);
	}

	void Collider2D::RebuildFixture()
	{
		if (fixture != nullptr && rigidBody != nullptr)
		{
			rigidBody->DestroyFixture(fixture);

			fixture = nullptr;
		}

		SearchRigidBody();

		if (rigidBody == nullptr)
			return;

		auto def = Physics::ToB2FixtureDef(physicsMaterial, reinterpret_cast<uintptr_t>(this));

		fixture = rigidBody->CreateFixture(&def);

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