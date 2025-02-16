#pragma once
#include "Components/GameComponent.h"
#include "Math/Vector2.h"
#include "Tools/Collisions/ICollisionVisitable.h"


class Transform;


class Collider2D : public GameComponent, public ICollisionVisitable
{
	friend class CollisionHandler;


protected:
	Transform* transform;

	Vector2F previousPosition;


public:
	Collider2D();

	virtual ~Collider2D() = default;


	virtual void Init();
	
	virtual void Update(float dt);

	virtual void Draw();

	virtual void Clean();


	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) override = 0;
};