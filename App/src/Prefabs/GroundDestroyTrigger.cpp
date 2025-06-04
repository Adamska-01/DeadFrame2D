#include "Prefabs/GroundDestroyTrigger.h"
#include <Components/Collisions/BoxCollider2D.h>
#include <Components/DestroyTrigger.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/Transform.h>
#include <Math/Vector2.h>
#include <SubSystems/Renderer.h>


void GroundDestroyTrigger::ConstructGameObject()
{
	auto renderTargetSize = Renderer::GetResolutionTarget();
	auto colliderThickness = 1.0f;

	transform->SetWorldPosition(Vector2F(renderTargetSize.x * 0.5f, renderTargetSize.y + colliderThickness * 0.5f));

	auto physicalMaterial = PhysicsMaterial
	{
		.isSensor = true
	};

	AddComponent<BoxCollider2D>(Vector2F(renderTargetSize.x, colliderThickness), Vector2F::Zero, 0.0f, physicalMaterial);
	AddComponent<RigidBody2D>(BodyDefinition2D(), 0.0f);
	AddComponent<DestroyTrigger>();
}