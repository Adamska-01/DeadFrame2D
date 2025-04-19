#include "Management/Scene.h"
#include "Management/SceneManager.h"


std::unique_ptr<Scene> SceneManager::currentScene;
std::unique_ptr<Scene> SceneManager::newLoadedScene;


SceneManager::SceneManager()
{
	currentScene = nullptr;
	newLoadedScene = nullptr;
}

SceneManager::~SceneManager()
{
	currentScene.reset();
}

void SceneManager::UpdateScene(float deltaTime) const
{
	if (currentScene == nullptr)
		return;

	currentScene->Update(deltaTime);
}

void SceneManager::DrawScene() const
{
	if (currentScene == nullptr)
		return;

	currentScene->Draw();
}

void SceneManager::LoadNewSceneIfAvailable()
{
	if (newLoadedScene == nullptr)
		return;

	if (currentScene != nullptr)
	{
		currentScene->Exit();

		currentScene.reset();
	}

	currentScene = std::move(newLoadedScene);

	currentScene->Enter();

	currentScene->Init();

	newLoadedScene.reset();
}

void SceneManager::LoadScene(std::unique_ptr<Scene> newGameScene)
{
	// Store new scene for later loading (at the end of the current frame)
	newLoadedScene = std::move(newGameScene);
}