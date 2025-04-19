#include "Management/Scene.h"
#include "Management/SceneManager.h"


std::unique_ptr<Scene> SceneManager::currentGameScene;


SceneManager::SceneManager()
{
	currentGameScene = nullptr;
}

SceneManager::~SceneManager()
{
	currentGameScene.reset();
}

void SceneManager::UpdateScene(float deltaTime) const
{
	if (currentGameScene == nullptr)
		return;

	currentGameScene->Update(deltaTime);
}

void SceneManager::DrawScene() const
{
	if (currentGameScene == nullptr)
		return;

	currentGameScene->Draw();
}

void SceneManager::LoadScene(std::unique_ptr<Scene> newGameScene)
{
	if (currentGameScene != nullptr)
	{
		currentGameScene->Exit();
	
		currentGameScene.reset();
	}

	currentGameScene = std::move(newGameScene);
	
	currentGameScene->Enter();

	currentGameScene->Init();
}