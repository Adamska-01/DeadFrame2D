#pragma once
#include <GameObject.h>


class Cannon : public GameObject
{
public:
	Cannon() = default;


	virtual void Init() override;
};