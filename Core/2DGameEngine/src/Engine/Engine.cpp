#include "Engine/Engine.h"
#include "SubSystems/Input/InputControls.h"
#include "SubSystems/Renderer.h"
#include "Tools/Helpers/EventHelpers.h"


Engine::Engine()
{
	isRunning = false;
	gameObjectsToInitialize.clear();

	engineSubSystems = std::make_unique<SubSystems>();
	engineSubSystems->InitializeSubSystems();

	gameObjectRegistry = std::make_unique<GameObjectRegistry>();

	ft.SetTargetFramerate(60);

	InputControls::Deserialize("Shared/Configurations/InputControls.json");

	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &Engine::GameObjectCreatedHandler));
}

void Engine::GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectCreatedEvent>(dispatchableEvent);

	auto gameObjectCreated = gameObjEvent == nullptr ? nullptr : gameObjEvent->gameObjectCreated;

	if (gameObjectCreated == nullptr)
		return;

	if (isRunning)
	{
		gameObjectCreated->Init();

		return;
	}

	// Store for late-initialization
	gameObjectsToInitialize.push_back(gameObjectCreated);
}

void Engine::Update(float deltaTime)
{
	gameObjectRegistry->Update(deltaTime);
}

void Engine::Draw()
{
	Renderer::ClearBuffer();

	gameObjectRegistry->Draw();

	Renderer::PresentBuffer();
}

std::optional<int> Engine::Run()
{
	// Some game objects might create other game objects in their Init function.
	// However, we want to ensure that only the game objects in the current
	// initialization queue are considered part of this initial phase. 
	// All the game objects created after this step are initalised at creation-time.
	auto currentToInitialize = gameObjectsToInitialize;
	gameObjectsToInitialize.clear();

	for (const auto& toInitialize : currentToInitialize)
	{
		auto toInitPtr = toInitialize.lock();

		if (toInitPtr == nullptr)
			continue;

		toInitPtr->Init();
	}

	isRunning = true;

	for (const auto& toInitialize : gameObjectsToInitialize)
	{
		auto toInitPtr = toInitialize.lock();

		if (toInitPtr == nullptr)
			continue;

		toInitPtr->Init();
	}

	currentToInitialize.clear();
	gameObjectsToInitialize.clear();

	while (isRunning)
	{
		ft.StartClock();

		//Looks for messages and return optional if QUIT
		if (const auto ecode = eventManager.ProcessEvents())
		{
			isRunning = false;

			return *ecode;
		}

		Update(ft.DeltaTime());
		Draw();

		//FPS and delay
		ft.EndClock();
		ft.DelayByFrameTime();
	}
}