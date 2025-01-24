#include "SubSystems/Input/Devices/InputDevice.h"


InputDevice::InputDevice()
	: assignedPlayer(PlayerInputSlot::NONE)
{
	currentInputStates.clear();
	lastInputStates.clear();
}

PlayerInputSlot InputDevice::GetAssignedPlayer() const
{
	return assignedPlayer;
}

void InputDevice::AssignedPlayer(PlayerInputSlot assignedPlayer)
{
	this->assignedPlayer = assignedPlayer;
}

bool InputDevice::IsKeyPressed(Uint8 inputKey) const
{
	auto currentIt = currentInputStates.find(inputKey);
	auto lastIt = lastInputStates.find(inputKey);

	if (currentIt != currentInputStates.end() && lastIt != lastInputStates.end())
		return currentIt->second && !lastIt->second;

	return false;
}

bool InputDevice::IsKeyHeld(Uint8 inputKey) const
{
	auto currentIt = currentInputStates.find(inputKey);

	if (currentIt != currentInputStates.end())
		return currentIt->second;

	return false;
}