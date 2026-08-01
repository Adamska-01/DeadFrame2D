#include "Engine/ECS/Component/Collisions/BoxCollider2D.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	BoxCollider2D::BoxCollider2D(Vector2F boxSize, Vector2F offsetFromCenter, float initialAngle, const PhysicsMaterial& physicsMaterial)
		: Collider2D(physicsMaterial)
	{
		SetNewBoxShape(boxSize, offsetFromCenter, initialAngle);
	}

	void BoxCollider2D::SetNewBoxShape(Vector2F boxSize, Vector2F offsetFromCenter, float initialAngle)
	{
		// TODO: the offset from center is actually offset from the RigidBody. Find a way to move the shape based on the game object's position.
		// Not sure how to do this, cause it would make sense when the RigidBody is not on the same GameObject as the collider, since in that case
		// moving the GameObject won't move the collider as well.
		physicsMaterial.shape = BoxShapeDefinition2D
		{
			.halfExtents = boxSize * 0.5f,
			.center = offsetFromCenter,
			.angle = initialAngle
		};

		MarkDirty();
	}
}