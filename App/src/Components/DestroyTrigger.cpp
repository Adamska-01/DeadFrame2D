#include "Components/DestroyTrigger.h"
#include "Data/Collision/CollisionInfo.h"
#include <Components/Collisions/Collider2D.h>
#include <GameObject.h>
#include <Tools/Helpers/EventHelpers.h>


DestroyTrigger::DestroyTrigger ()
	: collider(nullptr)
{
}

void DestroyTrigger::OnTriggerExitHandler(const CollisionInfo& collisionInfo)
{
	if (collisionInfo.otherCollider == nullptr
		|| collisionInfo.otherCollider->GetGameObject().lock() == nullptr)
		return;

	collisionInfo.otherCollider->GetGameObject().lock()->Destroy();
}

void DestroyTrigger::Init()
{
	collider = OwningObject.lock()->GetComponent<Collider2D>();

	collider->RegisterCollisionExitHandler(EventHelpers::BindFunction(this, &DestroyTrigger::OnTriggerExitHandler), reinterpret_cast<uintptr_t>(this));
}

void DestroyTrigger::Update(float deltaTime)
{
}

void DestroyTrigger::Draw()
{
}