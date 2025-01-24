#pragma once
#include "Generic/Bobble/BobbleColor.h"
#include "Generic/Bobble/BobbleNeighbors.h"
#include <GameObject.h>
#include <Math/Vector2.h>


class Bobble : public GameObject
{
private:
	BobbleNeighbors neighbors;

	BobbleColor color;


	void Bounce(Vector2F Normal);


public:
	Bobble(Vector2F startPos, BobbleColor color);
	
	virtual ~Bobble() = default;


	virtual void Update(float deltaTime) override;
};