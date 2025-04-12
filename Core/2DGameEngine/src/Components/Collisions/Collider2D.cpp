#include "Components/Collisions/Collider2D.h"
#include "Components/Transform.h"
#include "GameObject.h"


Collider2D::Collider2D()
	: transform(nullptr), startFramePosition(Vector2F::Zero)
{
}

void Collider2D::Init()
{
	transform = OwningObject->GetComponent<Transform>();
}

void Collider2D::Update(float dt)
{
	startFramePosition = transform->GetStartFramePosition();
}

void Collider2D::Draw()
{
}

void Collider2D::OnCollisionCallback(const CollisionInfo& collisionInfo)
{
	OnCollision(collisionInfo);
}

void Collider2D::RegisterCollisionHandler(const std::function<void(const CollisionInfo&)>& handler)
{
	OnCollision += handler;
}

void Collider2D::DeregisterEventHandler(const std::function<void(const CollisionInfo&)>& handler)
{
	OnCollision -= handler;
}

Transform* Collider2D::GetTranform() const
{
	return transform;
}

Vector2F Collider2D::GetStartFramePosition() const
{
	return startFramePosition;
}