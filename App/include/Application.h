#pragma once
#include <Engine/Engine.h>


class Bobble;
class GameMap;


class Application
{
private:
	std::unique_ptr<Engine> engine;

	std::weak_ptr<Bobble> bobble;

	std::weak_ptr<GameMap> gameMap;


public:
	Application();

	~Application() = default;


	std::optional<int> Run();
};