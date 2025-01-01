#include "Prefabs/Bobble.h"
#include "Components/Transform.h"
#include <Components/Sprite.h>
#include <Components/SpriteAnimator.h>


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

Bobble::~Bobble()
{
}

void Bobble::Bounce(Vector2F Normal)
{
}

void Bobble::Update(float dt)
{
	GameObject::Update(dt);

}

void Bobble::Draw()
{
	GameObject::Draw();

}

void Bobble::Clean()
{
	GameObject::Clean();

}