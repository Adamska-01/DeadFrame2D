#include "Coroutines/CoroutineScheduler.h"
#include "Management/Scene.h"
#include "Management/SceneManager.h"
#include "Tools/FrameTimer.h"


std::unique_ptr<Scene> SceneManager::currentScene;
std::function<std::unique_ptr<Scene>()> SceneManager::newSceneFactory;


SceneManager::SceneManager()
{
	currentScene = nullptr;
	newSceneFactory = nullptr;
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

void SceneManager::LateUpdateScene(float deltaTime) const
{
	if (currentScene == nullptr)
		return;

	currentScene->LateUpdate(deltaTime);
}

void SceneManager::DrawScene() const
{
	if (currentScene == nullptr)
		return;

	currentScene->Draw();
}

bool SceneManager::LoadNewSceneIfAvailable()
{
	if (newSceneFactory == nullptr)
		return false;

	CoroutineScheduler::Reset();
	FrameTimer::SetTimeScale(1.0f);

	if (currentScene != nullptr)
	{
		currentScene->Exit();

		currentScene.reset();
	}

	currentScene = newSceneFactory();

	currentScene->Enter();

	currentScene->Init();

	newSceneFactory = nullptr;

	return true;
}