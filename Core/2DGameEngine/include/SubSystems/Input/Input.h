#pragma once
#include "Devices/InputDevice.h"
#include "EventSystem/DispatchableEvent.h"
#include "SubSystems//Events/Interfaces/IEventProcessor.h"
#include "SubSystems/Input/Generic/PlayerInputSlot.h"
#include <memory>


class Input : public IEventProcessor
{
private:
	static std::unique_ptr<InputDevice> keyboardDevice;
	
	static std::unique_ptr<InputDevice> mouseDevice;

	static std::vector<std::unique_ptr<InputDevice>> controllerDevices;


	void DisconnectControllerHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


public:
	Input();

	~Input();


	std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	static bool IsButtonPressed(PlayerInputSlot playerSlot, const char* actionName);

	static bool IsButtonHeld(PlayerInputSlot playerSlot, const char* actionName);

	// TODO: Implement input axis
	static float GetAxisValue(PlayerInputSlot playerSlot, const char* actionName);
};