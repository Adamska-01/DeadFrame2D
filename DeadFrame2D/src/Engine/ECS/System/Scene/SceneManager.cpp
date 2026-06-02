#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Services/Time/FrameTimer.h"
#include "Engine/ECS/System/Scene/SceneManager.h"
#include "Engine/ECS/System/Scene/Scene.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	std::shared_ptr<Scene> SceneManager::currentScene = nullptr;
	std::function<std::shared_ptr<Scene>()> SceneManager::newSceneFactory = {};
	Data::CoreContext SceneManager::coreCtx;
	Data::ServiceContext SceneManager::serviceCtx;


	SceneManager::~SceneManager()
	{
		currentScene.reset();
	}


	void SceneManager::SetContexts(CoreContext coreCtx, ServiceContext serviceCtx)
	{
		this->coreCtx = coreCtx;
		this->serviceCtx = serviceCtx;
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

		coreCtx.coroutineScheduler->Reset();
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