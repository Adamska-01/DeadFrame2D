#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/Collisions/Collider2D.h"


namespace DF2D::Engine
{
	class DF2D_API CircleCollider2D : public Collider2D
	{
		TYPE_INFO(CircleCollider2D, Collider2D);


	private:
		b2CircleShape* shape;


		void DeleteShape();


	public:
		CircleCollider2D(
			float radius, 
			Core::Vector2F offsetFromCenter = Core::Vector2F::Zero, 
			const Data::PhysicsMaterial& physicsMaterial = Data::PhysicsMaterial());

		virtual ~CircleCollider2D() override;


		void SetNewCircleShape(float radius, Core::Vector2F offsetFromCenter);
	};
}