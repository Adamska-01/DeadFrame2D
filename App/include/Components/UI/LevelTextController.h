#pragma once
#include <Components/GameComponent.h>
#include <Components/UI/TextMesh.h>
#include <Coroutines/Task.h>


class GameManager;


class LevelTextController : public GameComponent
{
private:
	TextMesh* textMesh;

	GameManager* gameManager;


public:
	LevelTextController();

	virtual ~LevelTextController() override = default;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	Task DisplayLevelText(std::string text);

	Task DisplayEndLevelText(std::string text);
};