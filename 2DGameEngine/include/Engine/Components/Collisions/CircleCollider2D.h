#pragma once
#include "Core/Math/Vector2.h"
#include "Engine/Components/Collisions/Collider2D.h"


class CircleCollider2D : public Collider2D
{
private:
	b2CircleShape* shape;


	void DeleteShape();


public:
	CircleCollider2D(float radius, Vector2F offsetFromCenter = Vector2F::Zero, const PhysicsMaterial& physicsMaterial = PhysicsMaterial());

	virtual ~CircleCollider2D() override;

	
	void SetNewCircleShape(float radius, Vector2F offsetFromCenter);
};