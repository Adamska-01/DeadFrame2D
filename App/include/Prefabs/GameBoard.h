#pragma once
#include <GameObject.h>


class GameManager;


class GameBoard : public GameObject
{
private:
	GameManager* gameManager;


public:
	virtual void ConstructGameObject() override;


	GameManager* GetGameManager();
};