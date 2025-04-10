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
	transform->position = startPos;

	this->color = color;

	AddComponent<Sprite>(IDLE_BOBBLE_PATH);
	AddComponent<SpriteAnimator>()->SetProp(true, (int)color, 10, (int)BobbleColor::ALL_COLOURS, 5);
	AddComponent<CircleCollider2D>(Circle(Vector2F::Zero, 16));
	
	transform->Scale({ 2, 2 });
}

void Bobble::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	auto direction = Vector2F::Zero;

	if (Input::IsButtonHeld(PlayerInputSlot::PLAYER_1, "Left"))
	{
		direction.x = -1;
	}
	if (Input::IsButtonHeld(PlayerInputSlot::PLAYER_1, "Up"))
	{
		direction.y = -1;
	}
	if (Input::IsButtonHeld(PlayerInputSlot::PLAYER_1, "Down"))
	{
		direction.y = 1;
	}
	if (Input::IsButtonHeld(PlayerInputSlot::PLAYER_1, "Right"))
	{
		direction.x = 1;
	}

	auto movement = direction.Normalize() * deltaTime * 250.0f;

	transform->Translate(movement);
}

void Bobble::Bounce(Vector2F Normal)
{
}
