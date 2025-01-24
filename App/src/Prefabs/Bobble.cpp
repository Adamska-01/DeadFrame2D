#include "Prefabs/Bobble.h"
#include "Components/Transform.h"
#include <Components/Sprite.h>
#include <Components/SpriteAnimator.h>
#include <SubSystems/Input/Input.h>


const std::string IDLE_BOBBLE_PATH = "Assets/Sprites/IdleBobbles.png";

const std::string BLOW_BOBBLE_PATH = "Assets/Sprites/BlowBobbles.png";


Bobble::Bobble(Vector2F startPos, BobbleColor color)
{
	transform->position = startPos;

	this->color = color;

	AddComponent<Sprite>(IDLE_BOBBLE_PATH);
	auto animator = &AddComponent<SpriteAnimator>();

	animator->SetProp(true, (int)color, 10, (int)BobbleColor::ALL_COLOURS, 5);
	transform->Scale({ 3, 3 });
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

	transform->Translate(direction.Normalize() * deltaTime * 250.0f);
}

void Bobble::Bounce(Vector2F Normal)
{
}
