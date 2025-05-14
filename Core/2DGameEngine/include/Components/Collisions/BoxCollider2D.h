#pragma once
#include "Components/Collisions/Collider2D.h"


class BoxCollider2D : public Collider2D
{
private:
	b2PolygonShape* shape;


	void DeleteShape();


public:
	BoxCollider2D(Vector2F boxSize, Vector2F offsetFromCenter, float initialAngle, const PhysicsMaterial& physicsMaterial = PhysicsMaterial());

	virtual ~BoxCollider2D() override;


	void SetNewBoxShape(Vector2F boxSize, Vector2F offsetFromCenter, float initialAngle);
};