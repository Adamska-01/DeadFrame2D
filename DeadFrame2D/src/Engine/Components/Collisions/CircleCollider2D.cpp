#include "Converters/Physics/PhysicsConversions.h"
#include "Engine/Components/Collisions/CircleCollider2D.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Internal;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


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

		// TODO: the offset from center is actually offset from the RigidBody. Find a way to move the shape based on the game object's position.
		// Not sure how to do this, cause it would make sense when the RigidBody is not on the same GameObject as the collider, since in that case
		// moving the GameObject won't move the collider as well.
		shape = Physics::ToB2CircleShape(radius, offsetFromCenter);

		this->physicsMaterial.shape = shape;

		MarkDirty();
	}
}