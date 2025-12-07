#pragma once
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "Core/SubSystems/Systems/Input/Actions/RuntimeInputAction.h"
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <memory>


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

		virtual ~Input() override;

		Input(Input&&) = delete;


		Input& operator=(const Input&) = delete;

		Input& operator=(Input&&) = delete;


		std::shared_ptr<InputActionResolver> inputActionResolver;

		std::shared_ptr<DeviceManager> deviceManager;

		std::shared_ptr<InputUserManager> userManager;


		void DeviceAddedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);

		void DeviceRemovedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent);


		virtual void BeginFrame() override;

		virtual void Update(float deltaTime) override;

		virtual void EndUpdate() override;

		virtual void EndDraw() override;


	public:
		std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


		static std::shared_ptr<DeviceManager> Devices();

		static std::shared_ptr<InputUserManager> Users();

		static bool EnableActionMap(const std::string& actionMapName);

		static bool DisableActionMap(const std::string& actionMapName);

		static bool SwitchToActionMap(const std::string& actionMapName);


		static std::optional<RuntimeInputAction> TestActionQuery(const std::string& actionMapName, const std::string& actionName);
	};
}