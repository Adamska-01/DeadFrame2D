#pragma once
#include "Components/GameComponent.h"


class Transform;


class Collider2D : public GameComponent
{
	friend class CollisionHandler;


protected:
	Transform* transform;


public:
	Collider2D();

	virtual ~Collider2D() = default;


	virtual void Init();

	virtual void Update(float dt);

	virtual void Draw();

	virtual void Clean();


	virtual bool Accept(class ColliderVisitor& visitor, Collider2D& other) = 0;
};