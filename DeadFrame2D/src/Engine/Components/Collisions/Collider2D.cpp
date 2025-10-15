#include "Converters/Physics/PhysicsConversions.h"
#include "Core/SubSystems/Systems/CoroutineScheduler.h"
#include "Engine/Components/Collisions/Collider2D.h"
#include "Engine/Components/Physics/RigidBody2D.h"
#include "Engine/Components/Transform.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Helpers/Coroutines/CoroutineHelpers.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Converters;
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

	void Collider2D::GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		// This wouldn't work (dangling pointers)
		if (fixture != nullptr && rigidBody != nullptr)
			return;

		auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectCreatedEvent>(dispatchableEvent);

		if (gameObjEvent == nullptr || gameObjEvent->gameObjectCreated == nullptr)
			return;

		auto& target = gameObjEvent->gameObjectCreated;

		if (!target->IsChildOf(OwningObject))
			return;

		MarkDirty();
	}

	void Collider2D::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		// This wouldn't work (dangling pointers)
		if (fixture != nullptr && rigidBody != nullptr)
			return;

		auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

		if (!gameObjEvent || gameObjEvent->gameObjectDestroyed.lock() == nullptr)
			return;

		auto target = gameObjEvent->gameObjectDestroyed;

		if (!target.lock()->IsChildOf(OwningObject))
			return;

		MarkDirty();
	}

	Task Collider2D::Disable()
	{
		// Can't delete a fixture during a Box2D callback (e.g., BeginContact)
		// because the world is locked. Defer deletion until the next frame update.
		co_await WaitFrame();

		// TODO: instead of destroying the fixture, disable the body
		rigidBody->DestroyFixture(fixture);
		fixture = nullptr;
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

		rigidBody = OwningObject.lock()->GetComponent<RigidBody2D>();

		if (rigidBody != nullptr)
			return;

		rigidBody = OwningObject.lock()->GetComponentInParent<RigidBody2D>(true);
	}

	void Collider2D::OnGameObjectActiveStateChangedHandler(GameObject* obj, bool isActive)
	{
		if (isActive)
		{
			MarkDirty();
		}
		else
		{
			CoroutineScheduler::StartCoroutine(Disable());
		}
	}

	void Collider2D::Init()
	{
		transform = OwningObject.lock()->GetComponent<Transform>();

		SearchRigidBody();

		MarkDirty();
	}

	void Collider2D::Start()
	{

	}

	void Collider2D::Update(float dt)
	{
		if (isDirty)
		{
			RebuildFixture();
		}
	}

	void Collider2D::Draw()
	{
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