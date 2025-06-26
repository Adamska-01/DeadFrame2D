#pragma once
#include <GameObject.h>


class GameManager;


class GameBoard : public GameObject
{
private:
	GameManager* scoreManager;


public:
	virtual void ConstructGameObject() override;


	GameManager* GetScoreManager();
};