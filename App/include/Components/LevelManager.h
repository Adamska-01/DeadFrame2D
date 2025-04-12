#pragma once
#include "Generic/GameMode.h"
#include "Prefabs/BobbleGrid.h"
#include <array>
#include <Components/GameComponent.h>


class LevelManager : public GameComponent
{
private:
	GameMode gameMode;

	std::vector<std::string> levelSources;

	std::array<std::weak_ptr<BobbleGrid>, 2> bobbleGrid;


public:
	LevelManager(GameMode gameMode);

	virtual ~LevelManager() override;


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;
};