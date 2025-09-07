#include "Engine/EngineEvents/Events/SubSystems/Input/ControllerDisconnectedEvent.h"


ControllerDisconnectedEvent::ControllerDisconnectedEvent(const InputDevice* controllerDevice)
	: controllerDevice(controllerDevice)
{
}
