#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Services/Time/FrameTimer.h"
#include "Engine/ECS/System/Scene/Scene.h"
#include "Engine/ECS/System/Scene/SceneManager.h"



namespace DF2D::Engine
{
	using namespace DF2D::Core;


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