#pragma once
#include <Engine/Engine.h>


class Player;
class GameMap;
class LevelManagerObject;


class Application : public Engine
{
private:
	std::weak_ptr<Player> player;

	std::weak_ptr<GameMap> gameMap;

	std::weak_ptr<LevelManagerObject> levelManager;
	

public:
	Application();

	~Application() = default;
};