#pragma once
#include <Components/Collisions/Circle.h>
#include <Math/Vector2.h>


class CircleCollider2D
{
private:
	Circle collider;


public:
	CircleCollider2D() {};

	~CircleCollider2D() {};

	
	inline Circle GetCircle() { return collider; }

	inline void SetPos(Vector2 p_pos) { collider.position = p_pos; }
	
	inline void SetSize(float p_value) { collider.radius = p_value; }
};