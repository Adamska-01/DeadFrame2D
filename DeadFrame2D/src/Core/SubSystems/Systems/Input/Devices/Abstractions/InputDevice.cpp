#include "Core/SubSystems/Systems/Input/Devices/Abstractions/InputDevice.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Data;


	InputDevice::InputDevice()
		: assignedPlayer(PlayerInputSlot::NONE)
	{
		currentInputStates.clear();
		lastInputStates.clear();
	}

	void InputDevice::BeginInputFrame()
	{
		lastInputStates = currentInputStates;
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
}