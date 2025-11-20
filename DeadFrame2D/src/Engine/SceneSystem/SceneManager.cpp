#include "Core/Framerate/FrameTimer.h"
#include "Core/SubSystems/Systems/CoroutineScheduler.h"
#include "Engine/SceneSystem/Scene.h"
#include "Engine/SceneSystem/SceneManager.h"



namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


	std::shared_ptr<Scene> SceneManager::currentScene = nullptr;
	std::function<std::shared_ptr<Scene>()> SceneManager::newSceneFactory = {};


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
		if (!newSceneFactory)
			return false;

		CoroutineScheduler::Reset();
		FrameTimer::SetTimeScale(1.0f);

		if (currentScene != nullptr)
		{
			currentScene.reset();
		}

		currentScene = newSceneFactory();

		currentScene->Enter();

		currentScene->Init();

		newSceneFactory = {};

		return true;
	}	

	const Scene* SceneManager::GetActiveScene()
	{
		return currentScene.get();
	}
}