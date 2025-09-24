#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Collision/PhysicsMaterial.h"
#include "Engine/Components/Collisions/Collider2D.h"


namespace DeadFrame2D::Engine
{
	class BoxCollider2D : public Collider2D
	{
	private:
		b2PolygonShape* shape;


		void DeleteShape();


	public:
		BoxCollider2D(
			DeadFrame2D::Core::Vector2F boxSize, 
			DeadFrame2D::Core::Vector2F offsetFromCenter = DeadFrame2D::Core::Vector2F::Zero,
			float initialAngle = 0.0f, 
			const DeadFrame2D::Data::PhysicsMaterial& physicsMaterial = DeadFrame2D::Data::PhysicsMaterial());

		virtual ~BoxCollider2D() override;


		void SetNewBoxShape(DeadFrame2D::Core::Vector2F boxSize, DeadFrame2D::Core::Vector2F offsetFromCenter, float initialAngle);
	};
}