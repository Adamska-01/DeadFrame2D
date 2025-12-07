#include "Core/SubSystems/Systems/Input/Actions/inputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceManager.h"
#include "Core/SubSystems/Systems/Input/Input.h"
#include "Core/SubSystems/Systems/Input/User/InputUserManager.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceAddedEvent.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceRemovedEvent.h"
#include <cassert>
#include <iostream>


namespace DeadFrame2D::Core
{
	using namespace Shared::Models;

	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	Input* Input::instance = nullptr;


	Input::Input()
	{
		assert(instance == nullptr && "Input System was already initialized!");

		instance = this;

		inputActionResolver = std::make_shared<InputActionResolver>();
		deviceManager = std::make_shared<DeviceManager>(inputActionResolver);
		userManager = std::make_shared<InputUserManager>(deviceManager);
	
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(DeviceAddedEvent)), this, &Input::DeviceAddedEventHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(DeviceRemovedEvent)), this, &Input::DeviceRemovedEventHandler);
	}

	Input::~Input()
	{
		inputActionResolver.reset();
		deviceManager.reset();
		userManager.reset();

		instance = nullptr;

		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(DeviceAddedEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(DeviceRemovedEvent)), this);
	}

	void Input::DeviceAddedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent)
	{
		auto deviceAddedEvent = DispatchableEvent::SafeCast<DeviceAddedEvent>(dispatchableEvent);

		if (deviceAddedEvent == nullptr || deviceAddedEvent->GetDeviceAdded() == nullptr)
			return;

		auto deviceAdded = deviceAddedEvent->GetDeviceAdded();

		std::cout << "[Input] Device added: " << deviceAdded->Name() << " (ID: " << deviceAdded->ID() << ")" << std::endl;
	};

	void Input::DeviceRemovedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto deviceRemovedEvent = DispatchableEvent::SafeCast<DeviceRemovedEvent>(dispatchableEvent);

		if (deviceRemovedEvent == nullptr || deviceRemovedEvent->GetDeviceRemoved() == nullptr)
			return;

		auto deviceRemoved = deviceRemovedEvent->GetDeviceRemoved();

		for (const auto& user : userManager->GetAllUsers())
		{
			user->UnpairDevice(deviceRemoved->ID());
		}

		std::cout << "[Input] Device removed: " << deviceRemoved->Name() << " (ID: " << deviceRemoved->ID() << ")" << std::endl;
	}

	void Input::BeginFrame()
	{
		// The call order here matters!
		inputActionResolver->BeginFrame();
		deviceManager->BeginFrame();
	}

	void Input::PreUpdate(float deltaTime)
	{
		inputActionResolver->FinalizeActions();
	}

	void Input::EndUpdate(float deltaTime)
	{
	}

	void Input::EndDraw()
	{
	}


	std::optional<int> Input::ProcessEvents(const SDL_Event& sdlEvent)
	{
		deviceManager->HandleEvent(sdlEvent);
		
		return std::nullopt;
	}

	std::shared_ptr<DeviceManager> Input::Devices()
	{
		return instance->deviceManager;
	}

	std::shared_ptr<InputUserManager> Input::Users()
	{
		return instance->userManager;
	}

	bool Input::EnableActionMap(const std::string& actionMapName)
	{
		return instance->inputActionResolver->EnableActionMap(actionMapName);
	}

	bool Input::DisableActionMap(const std::string& actionMapName)
	{
		return instance->inputActionResolver->DisableActionMap(actionMapName);
	}

	bool Input::SwitchToActionMap(const std::string& actionMapName)
	{
		return instance->inputActionResolver->SwitchToActionMap(actionMapName);
	}

	std::optional<RuntimeInputAction> Input::TestActionQuery(const std::string& actionMapName, const std::string& actionName)
	{
		return instance->inputActionResolver->GetActionStateTEST(actionName);
	}
}