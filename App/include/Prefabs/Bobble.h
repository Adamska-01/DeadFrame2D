#pragma once
#include <GameObject.h>
#include <Generic/Bobble/BobbleColor.h>
#include <Generic/Bobble/BobbleNeighbors.h>
#include <Math/Vector2.h>


class Bobble : public GameObject
{
private:
	BobbleNeighbors neighbors;

	BobbleColor color;


	void Bounce(Vector2F Normal);


protected:
	Bobble(Vector2F startPos, BobbleColor color);


public:
	virtual ~Bobble() = default;
};