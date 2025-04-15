#pragma once
#include <Management/Scene.h>


class Player;
class GameMap;
class LevelManagerObject;


class GameScene : public Scene
{
private:
	std::weak_ptr<Player> player;

	std::weak_ptr<GameMap> gameMap;

	std::weak_ptr<LevelManagerObject> levelManager;


public:
	GameScene() = default;

	virtual ~GameScene() override = default;


	virtual void Enter() override;
};