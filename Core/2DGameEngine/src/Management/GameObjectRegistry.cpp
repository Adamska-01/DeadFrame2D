#include "Management/GameObjectRegistry.h"
#include "Management/GameScene.h"


std::unique_ptr<GameScene> GameObjectRegistry::currentGameScene;


GameObjectRegistry::GameObjectRegistry()
{
	currentGameScene = nullptr;
}

GameObjectRegistry::~GameObjectRegistry()
{
	currentGameScene.reset();
}

void GameObjectRegistry::Update(float deltaTime) const
{
	if (currentGameScene == nullptr)
		return;

	currentGameScene->Update(deltaTime);
}

void GameObjectRegistry::Draw() const
{
	if (currentGameScene == nullptr)
		return;

	currentGameScene->Draw();
}

void GameObjectRegistry::LoadScene(std::unique_ptr<GameScene> newGameScene)
{
	if (currentGameScene != nullptr)
	{
		currentGameScene->Exit();
	}

	currentGameScene = std::move(newGameScene);
	
	currentGameScene->Enter();
}