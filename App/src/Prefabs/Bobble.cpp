#include "Components/BobbleController.h"
#include "Prefabs/Bobble.h"
#include <Components/Collisions/CircleCollider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/Rendering/Sprite.h>
#include <Components/SpriteAnimator.h>
#include <Components/Transform.h>


const std::string IDLE_BOBBLE_PATH = "App/Assets/Sprites/IdleBobbles.png";

const std::string BLOW_BOBBLE_PATH = "App/Assets/Sprites/BlowBobbles.png";


Bobble::Bobble(Vector2F startPos, BobbleColor color)
{
	transform->SetWorldPosition(startPos);

	AddComponent<Sprite>(IDLE_BOBBLE_PATH);
	AddComponent<SpriteAnimator>()->SetProp(true, (int)color, 10, (int)BobbleColor::ALL_COLOURS, 5);
	AddComponent<CircleCollider2D>(16.0f);
	AddComponent<RigidBody2D>(BodyDefinition2D(), 0.0f);
	AddComponent<BobbleController>();
	
	transform->Scale({ 2, 2 });
}