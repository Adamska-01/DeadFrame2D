#include "Converters/Physics/PhysicsConversions.h"
#include "Engine/ECS/Component/Collisions/BoxCollider2D.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Internal;
	using namespace DeadFrame2D::Data;


	void BoxCollider2D::DeleteShape()
	{
		if (shape == nullptr)
			return;

		delete shape;
		shape = nullptr;
	}

	BoxCollider2D::BoxCollider2D(Vector2F boxSize, Vector2F offsetFromCenter, float initialAngle, const PhysicsMaterial& physicsMaterial)
		: Collider2D(physicsMaterial), shape(nullptr)
	{
		SetNewBoxShape(boxSize, offsetFromCenter, initialAngle);
	}

	BoxCollider2D::~BoxCollider2D()
	{
		DeleteShape();
	}

	void BoxCollider2D::SetNewBoxShape(Vector2F boxSize, Vector2F offsetFromCenter, float initialAngle)
	{
		DeleteShape();

		// TODO: the offset from center is actually offset from the RigidBody. Find a way to move the shape based on the game object's position.
		// Not sure how to do this, cause it would make sense when the RigidBody is not on the same GameObject as the collider, since in that case
		// moving the GameObject won't move the collider as well.
		shape = Physics::ToB2BoxShape(boxSize.x * 0.5f, boxSize.y * 0.5f, offsetFromCenter, initialAngle);

		this->physicsMaterial.shape = shape;

		MarkDirty();
	}
}