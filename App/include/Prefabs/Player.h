#pragma once
#include "Generic/PlayerIdentifier.h"
#include <GameObject.h>
#include <Math/Vector2.h>


class Sprite;
class Cannon;


class Player : public GameObject
{
private:
	PlayerIdentifier playerID;

	Sprite* sprite;

	Cannon* cannon;


public:
	Player(PlayerIdentifier playerID, Vector2F scale);
	
	virtual ~Player() override = default;


	virtual void Init() override;
	
	virtual void Update(float deltaTime) override;
};