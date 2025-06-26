#include "Components/DestroyTrigger.h"
#include "Data/Collision/CollisionInfo.h"
#include <Components/Collisions/Collider2D.h>
#include <GameObject.h>
#include <Tools/Helpers/EventHelpers.h>
#include <Tools/Helpers/Guards.h>


DestroyTrigger::DestroyTrigger ()
{
}

void DestroyTrigger::OnTriggerExitHandler(const CollisionInfo& collisionInfo)
{
	if (collisionInfo.otherGameObject.expired())
		return;

	collisionInfo.otherGameObject.lock()->Destroy();
}

void DestroyTrigger::Init()
{
	auto collider = OwningObject.lock()->GetComponent<Collider2D>();

	Tools::Helpers::GuardAgainstNull(collider, "DestroyTrigger::Init: No Collider2D found.");

	collider->RegisterContactExitHandler(EventHelpers::BindFunction(this, &DestroyTrigger::OnTriggerExitHandler), reinterpret_cast<uintptr_t>(this));
}

void DestroyTrigger::Start()
{

}

void DestroyTrigger::Update(float deltaTime)
{
}

void DestroyTrigger::Draw()
{
}