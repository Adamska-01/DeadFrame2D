#pragma once
#include "Components/Collisions/Circle.h"
#include "Components/GameComponent.h"
#include <Math/Vector2.h>


class Transform;


class CircleCollider2D : GameComponent
{
private:
	Circle collider;

	Transform* transform;


public:
	CircleCollider2D(Circle collider);

	~CircleCollider2D() = default;

	
	Circle GetCircle();

	void SetPos(Vector2F p_pos);
	
	void SetSize(float p_value);


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;
};