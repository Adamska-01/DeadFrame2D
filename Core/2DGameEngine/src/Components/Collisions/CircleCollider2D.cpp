#include "Components/Collisions/CircleCollider2D.h"
#include "Components/Transform.h"
#include "Constants/CommonColors.h"
#include "Factories/Concretions/Debugging/DebugCircleColliderDrawerFactory.h"
#include "Tools/Collisions/ICollisionVisitor.h"


CircleCollider2D::CircleCollider2D(Circle collider)
	: circle(collider), startFrameCircle(collider)
{
	debugCollisionDrawer = std::unique_ptr<IDebugColliderDrawer<CircleCollider2D>>(std::move(DebugCircleColliderDrawerFactory().CreateProduct(this)));
}

void CircleCollider2D::SetCirclePos(Circle& collisionCircle, const Vector2F& newPos)
{
	collisionCircle.position.x = newPos.x;
	collisionCircle.position.y = newPos.y;
}

const Circle& CircleCollider2D::GetCircle() const
{
	return circle;
}

const Circle& CircleCollider2D::GetStartFrameCircle() const
{
	return startFrameCircle;
}

void CircleCollider2D::SetPos(const Vector2F& pos)
{
	SetCirclePos(circle, pos);
}

void CircleCollider2D::SetSize(float newRadius)
{
	circle.radius = newRadius;

	startFrameCircle.radius = newRadius;
}

void CircleCollider2D::Init()
{
	Collider2D::Init();
}

void CircleCollider2D::Update(float dt)
{
	Collider2D::Update(dt);

}

void CircleCollider2D::Draw()
{
	Collider2D::Draw();

	SetCirclePos(circle, transform->position);

	debugCollisionDrawer->DrawCollider(CommonColors::WHITE);
}

bool CircleCollider2D::Accept(ICollisionVisitor& visitor, Collider2D* other)
{
	SetCirclePos(circle, transform->position);
	SetCirclePos(startFrameCircle, startFramePosition);

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