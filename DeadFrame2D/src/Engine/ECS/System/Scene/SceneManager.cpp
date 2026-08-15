#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Core/Services/Time/Abstractions/ITimeProvider.h"
// Order matters: SceneManager.h must precede Scene.h here, or the circular include between
// them resolves with Scene still incomplete when SceneManager's templates need it (C2027).
#include "Engine/ECS/System/Scene/SceneManager.h"
#include "Engine/ECS/System/Scene/Scene.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	SceneManager* SceneManager::activeInstance = nullptr;


	SceneManager::SceneManager()
		: coreCtx{},
		serviceCtx{}
	{
		if (activeInstance != nullptr)
		{
			throw std::runtime_error("A SceneManager instance is already active - only one may exist at a time.");
		}

		activeInstance = this;
	}

	SceneManager::~SceneManager()
	{
		currentScene.reset();

		if (activeInstance == this)
		{
			activeInstance = nullptr;
		}
	}


	SceneManager& SceneManager::Active()
	{
		return *Guard::AgainstNullAssignment(activeInstance, NAME_OF(activeInstance));
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

		serviceCtx.frameTimer->SetTimeScale(1.0f);

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
		return Active().currentScene.get();
	}


	Data::CoreContext DF2D::Engine::SceneManager::GetCoreContext()
	{
		return Active().coreCtx;
	}

	Data::ServiceContext DF2D::Engine::SceneManager::GetServiceContext()
	{
		return Active().serviceCtx;
	}
}