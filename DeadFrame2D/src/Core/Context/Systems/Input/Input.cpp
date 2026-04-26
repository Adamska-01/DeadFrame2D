#include "Core/Context/Systems/Input/Actions/InputActionResolver.h"
#include "Core/Context/Systems/Input/Devices/DeviceManager.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/SubSystems/Input/DeviceAddedEvent.h"
#include "Engine/Events/SubSystems/Input/DeviceRemovedEvent.h"
#include "Engine/Events/SubSystems/Input/InputUserCreatedEvent.h"
#include "Engine/Events/SubSystems/Input/InputUserDestroyedEvent.h"
#include <cassert>
#include <iostream>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Models;
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	Input* Input::instance = nullptr;


	Input::Input()
	{
		assert(instance == nullptr && "Input System was already initialized!");

		instance = this;

		auto inputActionResolver = std::make_shared<InputActionResolver>();
		actionsFrameLifecycle = std::static_pointer_cast<IInputFrameLifecycle>(inputActionResolver);
		actions = std::static_pointer_cast<IInputActions>(inputActionResolver);
		actionsHandler = std::static_pointer_cast<IInputActionHandler>(inputActionResolver);

		auto deviceManager = std::make_shared<DeviceManager>(actionsHandler.get());
		deviceFrameLifecycle = std::static_pointer_cast<IInputFrameLifecycle>(deviceManager);
		deviceProvider = std::static_pointer_cast<IInputDeviceProvider>(deviceManager);

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

		actionsFrameLifecycle.reset();
		actions.reset();
		actionsHandler.reset();
		deviceFrameLifecycle.reset();
		userManager.reset();

		instance = nullptr;
	}

	void Input::InputUserCreatedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto inputUserCreatedEvent = DispatchableEvent::SafeCast<InputUserCreatedEvent>(dispatchableEvent);
		
		if (inputUserCreatedEvent == nullptr)
			return;

		std::cout << "[Input] User created: "
			<< inputUserCreatedEvent->GetInputUserName()
			<< " (ID: " << inputUserCreatedEvent->GetInputUserID() << ")"
			<< std::endl;
	}

	void Input::InputUserDestroyedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto inputUserDestroyedEvent = DispatchableEvent::SafeCast<InputUserDestroyedEvent>(dispatchableEvent);
		
		if (inputUserDestroyedEvent == nullptr)
			return;

		std::cout << "[Input] User destroyed: "
			<< inputUserDestroyedEvent->GetInputUserName()
			<< " (ID: " << inputUserDestroyedEvent->GetInputUserID() << ")"
			<< std::endl;
	}

	void Input::DeviceAddedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto deviceAddedEvent = DispatchableEvent::SafeCast<DeviceAddedEvent>(dispatchableEvent);

		if (deviceAddedEvent == nullptr)
			return;

		std::cout << "[Input] Device added: " << deviceAddedEvent->GetDeviceName() << " (ID: " << deviceAddedEvent->GetDeviceID() << ")" << std::endl;
	};

	void Input::DeviceRemovedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto deviceRemovedEvent = DispatchableEvent::SafeCast<DeviceRemovedEvent>(dispatchableEvent);

		if (deviceRemovedEvent == nullptr)
			return;

		std::cout << "[Input] Device removed: " << deviceRemovedEvent->GetDeviceName() << " (ID: " << deviceRemovedEvent->GetDeviceID() << ")" << std::endl;
	}

	void Input::BeginFrame()
	{
		// The call order here matters!
		actionsFrameLifecycle->BeginFrame();
		deviceFrameLifecycle->BeginFrame();
	}

	void Input::PreUpdate(float deltaTime)
	{
		actionsFrameLifecycle->PreUpdate();
		deviceFrameLifecycle->PreUpdate();
	}

	void Input::EndUpdate(float deltaTime)
	{
	}

	void Input::EndDraw()
	{
	}

	IInputDeviceProvider* Input::Devices()
	{
		return instance->deviceProvider.get();
	}

	InputUserManager* Input::Users()
	{
		return instance->userManager.get();
	}

	IInputActions* Input::Actions()
	{
		return instance->actions.get();
	}
}