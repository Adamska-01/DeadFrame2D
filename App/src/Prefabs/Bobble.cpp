#include "Prefabs/Bobble.h"
#include <Components/Collisions/CircleCollider2D.h>
#include <Components/Sprite.h>
#include <Components/SpriteAnimator.h>
#include <Components/Transform.h>
#include <SubSystems/Input/Input.h>


const std::string IDLE_BOBBLE_PATH = "App/Assets/Sprites/IdleBobbles.png";

const std::string BLOW_BOBBLE_PATH = "App/Assets/Sprites/BlowBobbles.png";


Bobble::Bobble(Vector2F startPos, BobbleColor color)
{
	transform->SetWorldPosition(startPos);

	this->color = color;

	AddComponent<Sprite>(IDLE_BOBBLE_PATH);
	AddComponent<SpriteAnimator>()->SetProp(true, (int)color, 10, (int)BobbleColor::ALL_COLOURS, 5);
	AddComponent<CircleCollider2D>(Circle(Vector2F::Zero, 16));
	
	transform->Scale({ 2, 2 });
}

void Bobble::Bounce(Vector2F Normal)
{
}
