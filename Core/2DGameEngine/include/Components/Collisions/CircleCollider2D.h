#pragma once
#include "Components/Collisions/Collider2D.h"
#include "Components/Generic/Circle.h"
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


	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) override;

	virtual bool AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor);

	virtual bool AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor);

	virtual bool AcceptDispatch(TiledMapCompatibleCollider2D* other, ICollisionVisitor& visitor);
};