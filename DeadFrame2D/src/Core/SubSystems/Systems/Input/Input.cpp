#include "Core/SubSystems/Systems/Input/Actions/InputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceManager.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/SubSystems/Systems/Input/Input.h"
#include "Core/SubSystems/Systems/Input/User/InputUserManager.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceAddedEvent.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/DeviceRemovedEvent.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserCreatedEvent.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserDestroyedEvent.h"
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
		deviceManager = std::make_shared<DeviceManager>();
		userManager = std::make_shared<InputUserManager>();
	
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(InputUserCreatedEvent)), this, &Input::InputUserCreatedEventHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(InputUserDestroyedEvent)), this, &Input::InputUserDestroyedEventHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(DeviceAddedEvent)), this, &Input::DeviceAddedEventHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(DeviceRemovedEvent)), this, &Input::DeviceRemovedEventHandler);
	}

	Input::~Input()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(InputUserCreatedEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(InputUserDestroyedEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(DeviceAddedEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(DeviceRemovedEvent)), this);

		inputActionResolver.reset();
		deviceManager.reset();
		userManager.reset();

		instance = nullptr;
	}

	void Input::InputUserCreatedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto inputUserCreatedEvent = DispatchableEvent::SafeCast<InputUserCreatedEvent>(dispatchableEvent);
		// TODO: do something (Log?)
	}

	void Input::InputUserDestroyedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto inputUserDestroyedEvent = DispatchableEvent::SafeCast<InputUserDestroyedEvent>(dispatchableEvent);
		// TODO: do something (Log?)
	}

	void Input::DeviceAddedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
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

		std::cout << "[Input] Device removed: " << deviceRemoved->Name() << " (ID: " << deviceRemoved->ID() << ")" << std::endl;
	}

	void Input::BeginFrame()
	{
		// The call order here matters!
		static_cast<IInputActionsFrameManagement*>(inputActionResolver.get())->BeginFrame();
		deviceManager->BeginFrame();
	}

	void Input::PreUpdate(float deltaTime)
	{
		static_cast<IInputActionsFrameManagement*>(inputActionResolver.get())->FinalizeActions();
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

	DeviceManager* Input::Devices()
	{
		return instance->deviceManager.get();
	}

	InputUserManager* Input::Users()
	{
		return instance->userManager.get();
	}

	InputActionResolver* Input::Actions()
	{
		return instance->inputActionResolver.get();
	}
}