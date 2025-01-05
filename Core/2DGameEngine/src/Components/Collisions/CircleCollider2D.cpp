#include "Components/Collisions/CircleCollider2D.h"
#include "Components/Transform.h"
#include "GameObject.h"


CircleCollider2D::CircleCollider2D(Circle collider)
	: collider(collider)
{
}

Circle CircleCollider2D::GetCircle()
{
	return collider;
}

void CircleCollider2D::SetPos(Vector2F p_pos)
{
	collider.position = p_pos;
}

void CircleCollider2D::SetSize(float p_value)
{
	collider.radius = p_value;
}

void CircleCollider2D::Init()
{
	transform = &OwningObject->GetComponent<Transform>();
}

void CircleCollider2D::Update(float dt)
{
}

void CircleCollider2D::Draw()
{
}

void CircleCollider2D::Clean()
{
}
