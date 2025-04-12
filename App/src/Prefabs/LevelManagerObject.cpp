#include "Prefabs/LevelManagerObject.h"
#include <Components/LevelManager.h>


LevelManagerObject::LevelManagerObject(GameMode gameMode)
{
	AddComponent<LevelManager>(gameMode);
}