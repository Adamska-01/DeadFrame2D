#pragma once
#include "Components/GameComponent.h"
#include "Constants/BobbleConstants.h"
#include "Generic/Bobble/BobbleConnectionDirection.h"
#include "Generic/Bobble/BobbleColor.h"
#include <array>
#include <memory>


class GameObject;
class Transform;
class Sprite;
class SpriteAnimator;
class RigidBody2D;
class Collider2D;


class BobbleController : public GameComponent
{
private:
	bool partOfGrid;

	bool pendingDestruction;

	bool isDropping;

	bool isShot;

	BobbleColor bobbleColor;

	std::array<std::weak_ptr<GameObject>, BobbleConstants::MAX_BOBBLE_NEIGHBOURS> connectionList;

	Transform* transform;

	Sprite* sprite;
	
	SpriteAnimator* spriteAnimator;

	RigidBody2D* rigidBody;

	Collider2D* collider;


public:
	BobbleController(BobbleColor bobbleColor);

	virtual ~BobbleController() override = default;
	

	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void ShootBobble(Vector2F shootDirection);

	void PopBobble();

	void DropBobble();


	BobbleColor GetBobbleColor() const;

	bool IsDestructionPending() const;
	
	bool IsPartOfGrid() const;

	bool IsBobbleShot() const;

	std::weak_ptr<GameObject> GetConnectionAt(BobbleConnectionDirection connectionDirection) const;

	void SetColor(BobbleColor newColor);

	void SetPartOfGrid(bool partOfGrid);

	void SetConnectionAt(BobbleConnectionDirection connectionDirection, std::weak_ptr<GameObject> connection);
};