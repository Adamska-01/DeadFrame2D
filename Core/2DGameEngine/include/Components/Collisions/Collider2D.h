#pragma once
#include "Components/GameComponent.h"


class Transform;


class Collider2D : GameComponent
{
	friend class CollisionHandler;


protected:
	Transform* transform;


public:
	Collider2D();

	~Collider2D() = default;


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;


	virtual bool CollideWith(const Collider2D& other) const = 0;
};