#pragma once
#include <Components/GameComponent.h>
#include <memory>


class CollisionInfo;
class GameObject;
class Transform;


class Cannon : public GameComponent
{
private:
	Transform* transform;

	std::weak_ptr<GameObject> loadedBobble;

	float bobbleSpeed;

	bool bounceProcessed;


	void OnBobbleWallCollisionEnterHandler(const CollisionInfo& collisionInfo);

	void OnBobbleWallCollisionExitHandler(const CollisionInfo& collisionInfo);


public:
	Cannon();

	virtual ~Cannon() override;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void ShootBobble();

	void LoadBobble();
};