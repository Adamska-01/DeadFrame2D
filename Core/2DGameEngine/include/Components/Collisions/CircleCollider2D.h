#pragma once
#include "Components/Collisions/Collider2D.h"
#include "Components/Generic/Circle.h"
#include "Factories/Abstractions/Debugging/IDebugColliderDrawer.h"
#include <Math/Vector2.h>


class CircleCollider2D : public Collider2D
{
private:
	Circle circle;

	Circle startFrameCircle;
	
	std::unique_ptr<IDebugColliderDrawer<CircleCollider2D>> debugCollisionDrawer;

	
	void SetCirclePos(Circle& collisionCircle, const Vector2F& newPos);


public:
	CircleCollider2D(Circle collider);

	~CircleCollider2D() = default;

	
	const Circle& GetCircle() const;

	const Circle& GetStartFrameCircle() const;

	void SetPos(const Vector2F& pos);
	
	void SetSize(float newRadius);


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;


	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) override;

	virtual bool AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor);

	virtual bool AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor);

	virtual bool AcceptDispatch(TiledMapCompatibleCollider2D* other, ICollisionVisitor& visitor);
};