#include "Engine/ECS/Component/Collisions/CircleCollider2D.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	CircleCollider2D::CircleCollider2D(float radius, Vector2F offsetFromCenter, const PhysicsMaterial& physicsMaterial)
		: Collider2D(physicsMaterial)
	{
		SetNewCircleShape(radius, offsetFromCenter);
	}

	void CircleCollider2D::SetNewCircleShape(float radius, Vector2F offsetFromCenter)
	{
		// TODO: the offset from center is actually offset from the RigidBody. Find a way to move the shape based on the game object's position.
		// Not sure how to do this, cause it would make sense when the RigidBody is not on the same GameObject as the collider, since in that case
		// moving the GameObject won't move the collider as well.
		physicsMaterial.shape = CircleShapeDefinition2D
		{
			.radius = radius,
			.center = offsetFromCenter
		};

		MarkDirty();
	}
}