#pragma once
#include <Components/GameComponent.h>
#include <memory>
#include <SDL.h>


class RigidBody2D;
class BoxCollider2D;
class BobbleGrid;


class GridCeiling : public GameComponent
{
private:
	std::shared_ptr<SDL_Texture> spriteTexture;

	SDL_Rect srcCealingTileTop;

	SDL_Rect srcCealingTileBottom;

	int ceilingLevel;

	int ceilingTileSize;

	int ceilingWidth;

	BoxCollider2D* collider;

	BobbleGrid* bobbleGrid;


	void OnCeilingCollisionEnterHandler(const CollisionInfo& collisionInfo);


public:
	GridCeiling();

	virtual ~GridCeiling() override;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetCeilingTileSize(int ceilingTileSize);

	void CollapseCeiling();

	void ResetCeiling();

	int GetCeilingLevel();
};