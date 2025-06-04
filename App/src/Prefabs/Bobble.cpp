#include "Components/BobbleController.h"
#include "Constants/AssetPaths.h"
#include "Prefabs/Bobble.h"
#include <Components/Collisions/CircleCollider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/Rendering/Sprite.h>
#include <Components/SpriteAnimator.h>
#include <Components/Transform.h>


Bobble::Bobble(Vector2F startPos, BobbleColor color)
	: startPos(startPos)
{
	AddComponent<Sprite>(AssetPaths::Sprites::IDLE_BOBBLE_PATH);
	AddComponent<SpriteAnimator>();
	AddComponent<CircleCollider2D>(16.0f);
	AddComponent<RigidBody2D>(BodyDefinition2D(), 0.0f);
	AddComponent<BobbleController>(color);
}

void Bobble::ConstructGameObject()
{
	transform->SetWorldPosition(startPos);
	transform->Scale({ 2, 2 });
}