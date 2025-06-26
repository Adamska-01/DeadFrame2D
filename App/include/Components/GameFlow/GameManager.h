#pragma once
#include <Components/GameComponent.h>
#include <Coroutines/Task.h>
#include <Generic/ScoreCounter.h>
#include <string>
#include <vector>


class Cannon;
class BobbleGrid;
class GridCeiling;
class MenuManager;
class LevelTextController;


class GameManager : public GameComponent
{
private:
	std::vector<std::string> levelSources;

	int tileSize;

	int currentLevelIndex;

	Cannon* player;

	BobbleGrid* bobbleGrid;

	GridCeiling* gridCeiling;

	MenuManager* menuManager;

	LevelTextController* levelTextController;

	bool inputAllowed;

	ScoreCounter scoreCounter;


public:
	GameManager();

	virtual ~GameManager() override = default;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	bool IsInputAllowed();

	void SetScoreTextMesh(TextMesh* textMesh);

	void AddScore(int scoreToAdd);

	Task StartNextLevel(bool displayEndLevelText);

	void ShowGameOverScreen();
};