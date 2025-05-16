#pragma once
#include <GameObject.h>


class Player : public GameObject
{
public:
	Player();
	
	virtual ~Player() override = default;


	virtual void Init() override;
	
	virtual void Update(float deltaTime) override;
};