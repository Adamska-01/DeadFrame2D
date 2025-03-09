#pragma once
#include "Components/GameComponent.h"
#include "Data/Collision/CollisionInfo.h"
#include "Math/Vector2.h"
#include "Tools/Collisions/ICollisionVisitable.h"
#include "Tools/MulticastDelegate.h"


class Transform;


class Collider2D : public GameComponent, public ICollisionVisitable
{
protected:
	Transform* transform;

	Vector2F startFramePosition;

	MulticastDelegate<const CollisionInfo&> OnCollision;


public:
	Collider2D();

	virtual ~Collider2D() = default;


	virtual void Init();
	
	virtual void Update(float dt);

	virtual void Draw();


	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) override = 0;


	void OnCollisionCallback(const CollisionInfo& collisionInfo);

	void RegisterCollisionHandler(const std::function<void(const CollisionInfo&)>& handler);
	
	void DeregisterEventHandler(const std::function<void(const CollisionInfo&)>& handler);


	Transform* GetTranform() const;

	Vector2F GetStartFramePosition() const;
};