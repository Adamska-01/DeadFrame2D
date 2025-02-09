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


	virtual void Init() override;

	virtual void Update(float dt) = 0;

	virtual void Draw() = 0;

	virtual void Clean() = 0;


	virtual bool Accept(class ColliderVisitor& visitor, Collider2D& other) = 0;
};