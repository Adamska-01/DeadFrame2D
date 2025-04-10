#pragma once
#include <Engine/Engine.h>


class Cannon;
class Bobble;
class GameMap;
class BobbleGrid;


class Application : public Engine
{
private:
	std::weak_ptr<Cannon> cannon;

	std::weak_ptr<Bobble> bobble;

	std::weak_ptr<GameMap> gameMap;
	
	std::weak_ptr<BobbleGrid> bobbleGrid;


public:
	Application();

	~Application() = default;
};