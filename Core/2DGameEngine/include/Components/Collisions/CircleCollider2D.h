#pragma once
#include "Components/Generic/Circle.h"
#include "Components/Collisions/Collider2D.h"
#include <Math/Vector2.h>


class CircleCollider2D : public Collider2D
{
private:
	Circle collider;


public:
	CircleCollider2D(Circle collider);

	~CircleCollider2D() = default;

	
	Circle GetCircle() const;

	void SetPos(Vector2F p_pos);
	
	void SetSize(float p_value);


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;


	virtual bool Accept(class ColliderVisitor& visitor, Collider2D& other) override;
};