#pragma once
#include "Generic/Bobble/BobbleColor.h"
#include <GameObject.h>
#include <Math/Vector2.h>


class Bobble : public GameObject
{
public:
	Bobble(Vector2F startPos, BobbleColor color);
	
	virtual ~Bobble() = default;
};