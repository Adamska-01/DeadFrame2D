#pragma once
#include "Generic/GameMode.h"
#include <GameObject.h>


class LevelManagerObject : public GameObject
{
public:
	LevelManagerObject(GameMode gameMode);

	virtual ~LevelManagerObject() override = default;
};