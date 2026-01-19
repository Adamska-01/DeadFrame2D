#pragma once
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class DispatchableEvent;
}


namespace DeadFrame2D::Core
{
	class InputActionResolver;
	class InputUserManager;
	class DeviceManager;


	class DF2D_API Input : public IEventProcessor, public ISubSystem
	{
		friend class SubSystemManager;


	private:
		static Input* instance;


		Input();

		~Input() override;

		Input(Input&&) = delete;


		Input& operator=(const Input&) = delete;

		Input& operator=(Input&&) = delete;


		std::shared_ptr<InputActionResolver> inputActionResolver;

		std::shared_ptr<DeviceManager> deviceManager;

		std::shared_ptr<InputUserManager> userManager;


		void InputUserCreatedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);

		void InputUserDestroyedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);

		void DeviceAddedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);

		void DeviceRemovedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	public:
		// TODO: Create a wrapper around all these managers that exposes only the client API
		static DeviceManager* Devices();

		static InputUserManager* Users();

		static InputActionResolver* Actions();
	};
}