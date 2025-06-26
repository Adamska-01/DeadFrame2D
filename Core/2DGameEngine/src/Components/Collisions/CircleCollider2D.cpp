#include "Components/Collisions/CircleCollider2D.h"
#include "Tools/Helpers/Physics/PhysicsShapeCreators.h"


CircleCollider2D::CircleCollider2D(float radius, Vector2F offsetFromCenter, const PhysicsMaterial& physicsMaterial)
	: Collider2D(physicsMaterial), shape(nullptr)
{
	SetNewCircleShape(radius, offsetFromCenter);
}

CircleCollider2D::~CircleCollider2D()
{
	DeleteShape();
}

void CircleCollider2D::DeleteShape()
{
	if (shape != nullptr)
	{
		delete shape;
		shape = nullptr;
	}
}

void CircleCollider2D::SetNewCircleShape(float radius, Vector2F offsetFromCenter)
{
	DeleteShape();

	shape = PhysicsShapeCreators::CreateCircleShape(radius, offsetFromCenter);

	this->physicsMaterial.shape = shape;

	MarkDirty();
}