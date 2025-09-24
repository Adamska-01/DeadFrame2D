#include "Engine/Components/Collisions/BoxCollider2D.h"
#include "Utilities/Helpers/Physics/PhysicsShapeCreators.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


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
	
		shape = CreateBoxShape(boxSize.x * 0.5f, boxSize.y * 0.5f, offsetFromCenter, initialAngle);

		this->physicsMaterial.shape = shape;

		MarkDirty();
	}
}