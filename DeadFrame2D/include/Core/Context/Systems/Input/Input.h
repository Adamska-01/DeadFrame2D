#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Services/Events/Abstractions/ISystemEventSink.h"
#include "DF2D_API.h"
#include <memory>


namespace DF2D::Engine
{
	class EventDispatcher;
}


namespace DF2D::Models
{
	struct InputActionMapBucket;
}


namespace DF2D::Core
{
	class IInputActions;
	class IInputDeviceProvider;
	class IInputUsers;
	class InputActionResolver;
	class DeviceManager;
	class InputUserManager;


	class DF2D_API Input : public ICoreSystem, public ISystemEventSink
	{
	private:
		std::unique_ptr<InputActionResolver> actionResolver;

		std::unique_ptr<DeviceManager> deviceManager;

		std::unique_ptr<InputUserManager> userManager;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		void OnSystemEvent(const Data::SystemEvent& systemEvent) override;


	public:
		Input(Models::InputActionMapBucket actionMapBucket, Engine::EventDispatcher& eventDispatcher);

		~Input() override;

		Input(const Input&) = delete;

		Input(Input&&) = delete;

		Input& operator=(const Input&) = delete;

		Input& operator=(Input&&) = delete;


		IInputDeviceProvider* Devices();

		IInputUsers* Users();

		IInputActions* Actions();
	};
}