#include "Prefabs/Bobble.h"
#include "Components/Transform.h"
#include <Components/Sprite.h>
#include <Components/SpriteAnimator.h>


const std::string IDLE_BOBBLE_PATH = "Assets/Sprites/IdleBobbles.png";

const std::string BLOW_BOBBLE_PATH = "Assets/Sprites/BlowBobbles.png";


Bobble::Bobble(Vector2 startPos, BobbleColor color)
{
	transform->position = startPos;

	this->color = color;

	AddComponent<Sprite>(IDLE_BOBBLE_PATH);
	AddComponent<SpriteAnimator>();
}

Bobble::~Bobble()
{
}

void Bobble::Bounce(Vector2 Normal)
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