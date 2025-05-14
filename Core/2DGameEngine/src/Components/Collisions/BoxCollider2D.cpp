#include "Components/Collisions/BoxCollider2D.h"
#include "Tools/Helpers/Physics/PhysicsShapeCreators.h"


BoxCollider2D::BoxCollider2D(Vector2F boxSize, Vector2F offsetFromCenter, float initialAngle, const PhysicsMaterial& physicsMaterial)
	: Collider2D(physicsMaterial), shape(nullptr)
{
	SetNewBoxShape(boxSize, offsetFromCenter, initialAngle);
}

void BoxCollider2D::SetNewBoxShape(Vector2F boxSize, Vector2F offsetFromCenter, float initialAngle)
{
	if (shape != nullptr)
	{
		delete shape;
		shape = nullptr;
	}
	
	shape = PhysicsShapeCreators::CreateBoxShape(boxSize.x * 0.5f, boxSize.y * 0.5f, offsetFromCenter, initialAngle);

	this->physicsMaterial.shape = shape;

	MarkDirty();
}