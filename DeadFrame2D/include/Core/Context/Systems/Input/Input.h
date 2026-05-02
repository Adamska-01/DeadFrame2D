#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "DF2D_API.h"
#include <memory>


namespace DF2D::Engine
{
	class DispatchableEvent;
}


namespace DF2D::Core
{
	class IInputFrameLifecycle;
	class IInputActions;
	class IInputActionHandler;
	class IInputDeviceProvider;
	class InputUserManager;


	class DF2D_API Input : public ICoreSystem
	{
		friend class SystemInitializer;


	private:
		static Input* instance;


		Input();

		~Input() override;

		Input(Input&&) = delete;


		Input& operator=(const Input&) = delete;

		Input& operator=(Input&&) = delete;


		std::shared_ptr<IInputFrameLifecycle> actionsFrameLifecycle;

		std::shared_ptr<IInputActions> actions;

		std::shared_ptr<IInputActionHandler> actionsHandler;

		std::shared_ptr<IInputFrameLifecycle> deviceFrameLifecycle;

		std::shared_ptr<IInputDeviceProvider> deviceProvider;

		std::shared_ptr<InputUserManager> userManager;


		void InputUserCreatedEventHandler(std::shared_ptr<Engine::DispatchableEvent> dispatchableEvent);

		void InputUserDestroyedEventHandler(std::shared_ptr<Engine::DispatchableEvent> dispatchableEvent);

		void DeviceAddedEventHandler(std::shared_ptr<Engine::DispatchableEvent> dispatchableEvent);

		void DeviceRemovedEventHandler(std::shared_ptr<Engine::DispatchableEvent> dispatchableEvent);


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		static IInputDeviceProvider* Devices();

		static InputUserManager* Users();

		static IInputActions* Actions();
	};
}