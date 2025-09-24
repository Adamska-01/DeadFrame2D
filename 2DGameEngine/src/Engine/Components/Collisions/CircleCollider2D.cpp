#include "Engine/Components/Collisions/CircleCollider2D.h"
#include "Utilities/Helpers/Physics/PhysicsShapeCreators.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
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

		shape = CreateCircleShape(radius, offsetFromCenter);

		this->physicsMaterial.shape = shape;

		MarkDirty();
	}
}