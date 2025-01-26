#include "Components/Collisions/Collider2D.h"
#include "Components/Transform.h"
#include "GameObject.h"


Collider2D::Collider2D()
	: transform(nullptr)
{
}

void Collider2D::Init()
{
	transform = OwningObject->GetComponent<Transform>();
}

void Collider2D::Update(float dt)
{
}

void Collider2D::Draw()
{
}

void Collider2D::Clean()
{
}
