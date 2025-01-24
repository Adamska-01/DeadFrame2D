#pragma once
#include "SubSystems/Input/Generic/PlayerInputSlot.h"
#include <SDL_events.h>
#include <unordered_map>




class InputDevice
{
protected:
	PlayerInputSlot assignedPlayer;

	std::unordered_map<Uint8, bool> currentInputStates;

	std::unordered_map<Uint8, bool> lastInputStates;


	InputDevice();


public:
	virtual void ProcessEvent(const SDL_Event& controllerEvent) = 0;


	PlayerInputSlot GetAssignedPlayer() const;
	
	void AssignedPlayer(PlayerInputSlot assignedPlayer);

	bool IsKeyPressed(Uint8 inputKey) const;

	bool IsKeyHeld(Uint8 inputKey) const;
};