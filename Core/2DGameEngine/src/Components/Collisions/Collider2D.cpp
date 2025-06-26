#include "Components/Collisions/Collider2D.h"
#include "Components/Physics/RigidBody2D.h"
#include "Components/Transform.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "GameObject.h"
#include "Tools/Helpers/Physics/PhysicsConversion.h"


Collider2D::Collider2D(const PhysicsMaterial& physicsMaterial)
	: transform(nullptr), 
	fixture(nullptr), 
	rigidBody(nullptr), 
	physicsMaterial(physicsMaterial)
{
}

Collider2D::~Collider2D()
{
	if (fixture == nullptr || rigidBody == nullptr)
		return;
	
	// THIS CAUSES NULL REF ERRORS! MAKE COMPONENTS SMART POINTERS!!!!!!!!
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

	auto def = PhysicsConversion::ToB2FixtureDef(physicsMaterial, reinterpret_cast<uintptr_t>(this));

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

	rigidBody = OwningObject.lock()->GetComponentInParent<RigidBody2D>();
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

Transform* Collider2D::GetTranform() const
{
	return transform;
}

PhysicsMaterial Collider2D::GetPhysicsMaterial()
{
	return physicsMaterial;
}