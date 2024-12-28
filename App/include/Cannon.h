#pragma once
#include "Stats.h"
#include <Gameobject.h>
#include <SubSystems/TextureManager.h>


class Cannon : public GameObject
{
private:
	Transform* CannonT;

	Bobble* bobble;
	
	SDL_Texture* arrowTexture;
	
	SDL_Texture* cannonTexture;
	
	Vector2 RotationPos;	//Rotation point (Cannon)
	
	Vector2 Direction;		//Direction vector (Arrow)

	SDL_Rect canRect;

	SDL_Rect arrowRect;

	SDL_Point rotPoint;		//SDL has a data type for the rotation point 

	float rotationSpeed;
	
	float minRot;
	
	float maxRot;

	//Controller number
	int WhatPlayer;

	bool gameOver;


public:
	Cannon(Vector2 pos, int whatPlayer);

	~Cannon();


	void Update(float dt) override;
	
	void Draw() override;
	
	void Clean() override;


	void SetGameover(bool value);

	bool GetGameOver();
	
	Bobble* GetBobble();
};