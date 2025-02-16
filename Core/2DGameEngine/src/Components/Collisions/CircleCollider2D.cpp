#include "Components/Collisions/CircleCollider2D.h"
#include "Components/Transform.h"
#include "Tools/Collisions/ICollisionVisitor.h"


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
	Collider2D::Update(dt);

	const auto& pos = transform->position;

	collider.position.x = previousPosition.x;
	collider.position.y = previousPosition.y;
}

void CircleCollider2D::Draw()
{
}

void CircleCollider2D::Clean()
{
}

bool CircleCollider2D::Accept(ICollisionVisitor& visitor, Collider2D* other)
{
	return other->AcceptDispatch(this, visitor);
}

bool CircleCollider2D::AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor)
{
	return visitor.Visit(this, other);
}

bool CircleCollider2D::AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor)
{
	return visitor.Visit(this, other);
}

bool CircleCollider2D::AcceptDispatch(TiledMapCompatibleCollider2D* other, ICollisionVisitor& visitor)
{
	return visitor.Visit(this, other);
}