#include "Components/Collisions/CircleCollider2D.h"
#include "Components/Collisions/CollisionHandler.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include <Components/Collisions/BoxCollider2D.h>


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

bool CircleCollider2D::CollideWith(const Collider2D& other) const
{
	auto collisionPoint = Vector2F();

	if (auto circle = dynamic_cast<const CircleCollider2D*>(&other))
		return CollisionHandler::CircleToCircleCollision(*this, *circle);
	else if (auto box = dynamic_cast<const BoxCollider2D*>(&other))
		return CollisionHandler::CircleToBoxCollision(*this, *box, collisionPoint);

	return false;
}