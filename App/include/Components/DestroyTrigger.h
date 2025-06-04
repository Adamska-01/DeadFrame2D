#pragma once
#include "Components/GameComponent.h"


struct CollisionInfo;
class Collider2D;


class DestroyTrigger : public GameComponent
{
private:
	Collider2D* collider;


	void OnTriggerExitHandler(const CollisionInfo& collisionInfo);


public:
	DestroyTrigger();


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;
};