#pragma once
#include "Generic/Bobble/BobbleColor.h"
#include <Components/GameComponent.h>
#include <Math/Vector2.h>
#include <memory>


class CollisionInfo;
class Transform;
class GameManager;
class BobbleGrid;
class GameObject;
class DispatchableEvent;


class Cannon : public GameComponent
{
private:
	static constexpr int MAX_COLORS = (int)BobbleColor::MAX_COLORS;


	Transform* transform;

	GameManager* gameManager;

	BobbleGrid* bobbleGrid;

	std::weak_ptr<GameObject> loadedBobble;

	std::weak_ptr<GameObject> pendingBobble;

	float bobbleSpeed;

	bool bounceProcessed;

	int currentColor;


	BobbleColor GetAvailableColor();


	void OnBobbleWallCollisionEnterHandler(const CollisionInfo& collisionInfo);

	void OnBobbleWallCollisionExitHandler(const CollisionInfo& collisionInfo);

	void LoadNewBobbleEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

	void ProcessRotationInput(float deltaTime);

	std::weak_ptr<GameObject> LoadBobble();

	std::weak_ptr<GameObject> CreateNewBobble(Vector2F startPos);


public:
	Cannon();

	virtual ~Cannon() override;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void ShootBobble();

	std::weak_ptr<GameObject> ResetLoadedBobble();
};