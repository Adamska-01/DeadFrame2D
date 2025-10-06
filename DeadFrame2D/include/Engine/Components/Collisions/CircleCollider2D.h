#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/Components/Collisions/Collider2D.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API CircleCollider2D : public Collider2D
	{
	private:
		b2CircleShape* shape;


		void DeleteShape();


	public:
		CircleCollider2D(
			float radius, 
			DeadFrame2D::Core::Vector2F offsetFromCenter = DeadFrame2D::Core::Vector2F::Zero, 
			const DeadFrame2D::Data::PhysicsMaterial& physicsMaterial = DeadFrame2D::Data::PhysicsMaterial());

		virtual ~CircleCollider2D() override;


		void SetNewCircleShape(float radius, DeadFrame2D::Core::Vector2F offsetFromCenter);
	};
}