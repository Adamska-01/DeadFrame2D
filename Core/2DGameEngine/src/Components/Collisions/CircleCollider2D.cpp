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

void CircleCollider2D::SetPos(const Vector2F& pos)
{
	collider.position.x = pos.x;
	collider.position.y = pos.y;
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

	SetPos(startFramePosition);
}

void CircleCollider2D::Draw()
{
}

bool CircleCollider2D::Accept(ICollisionVisitor& visitor, Collider2D* other)
{
	SetPos(transform->position);

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