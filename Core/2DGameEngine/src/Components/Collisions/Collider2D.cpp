#include "Components/Collisions/Collider2D.h"
#include "Components/Transform.h"
#include "GameObject.h"


Collider2D::Collider2D()
	: transform(nullptr), previousPosition(Vector2F::Zero)
{
}

void Collider2D::Init()
{
	transform = OwningObject->GetComponent<Transform>();
}

void Collider2D::Update(float dt)
{
	previousPosition.x = transform->position.x;
	previousPosition.y = transform->position.y;
}

void Collider2D::Draw()
{
}

void Collider2D::Clean()
{
}
