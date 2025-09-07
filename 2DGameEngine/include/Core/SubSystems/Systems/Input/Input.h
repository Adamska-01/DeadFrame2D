#pragma once
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"
#include "Data/Input/PlayerInputSlot.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <memory>
#include <vector>


class Input : public IEventProcessor, public ISubSystem
{
	friend class SubSystems;


private:
	Input();

	virtual ~Input() override;

	Input(Input&&) = delete;


	Input& operator=(const Input&) = delete;

	Input& operator=(Input&&) = delete;


	static std::unique_ptr<InputDevice> keyboardDevice;
	
	static std::unique_ptr<InputDevice> mouseDevice;

	static std::vector<std::unique_ptr<InputDevice>> controllerDevices;


	void DisconnectControllerHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	virtual void Update(float deltaTime) override;

	virtual void BeginFrame() override;

	virtual void EndUpdate() override;

	virtual void EndDraw() override;


public:
	std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	static bool IsButtonPressed(PlayerInputSlot playerSlot, const char* actionName);

	static bool IsButtonHeld(PlayerInputSlot playerSlot, const char* actionName);

	// TODO: Implement input axis
	static float GetAxisValue(PlayerInputSlot playerSlot, const char* actionName);
};