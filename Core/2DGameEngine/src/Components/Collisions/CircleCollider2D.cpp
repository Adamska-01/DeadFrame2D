#include "Components/Collisions/CircleCollider2D.h"
#include "Tools/Collisions/ColliderVisitor.h"


CircleCollider2D::CircleCollider2D(Circle collider)
	: collider(collider)
{
}

Circle CircleCollider2D::GetCircle() const
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
	Collider2D::Init();
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

bool CircleCollider2D::Accept(ColliderVisitor& visitor, Collider2D& other)
{
	return visitor.Visit(*this, other);
}
