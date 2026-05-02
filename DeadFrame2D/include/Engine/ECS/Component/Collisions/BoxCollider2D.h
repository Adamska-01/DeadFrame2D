#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/Collision/PhysicsMaterial.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/Collisions/Collider2D.h"


namespace DF2D::Engine
{
	class DF2D_API BoxCollider2D : public Collider2D
	{
		TYPE_INFO(BoxCollider2D, Collider2D);
	
	
	private:
		b2PolygonShape* shape;


		void DeleteShape();


	public:
		BoxCollider2D(
			Core::Vector2F boxSize, 
			Core::Vector2F offsetFromCenter = Core::Vector2F::Zero,
			float initialAngle = 0.0f, 
			const Data::PhysicsMaterial& physicsMaterial = Data::PhysicsMaterial());

		virtual ~BoxCollider2D() override;


		void SetNewBoxShape(Core::Vector2F boxSize, Core::Vector2F offsetFromCenter, float initialAngle);
	};
}