#include "EventSystem/Events/Input/ControllerDisconnectedEvent.h"


ControllerDisconnectedEvent::ControllerDisconnectedEvent(const InputDevice* controllerDevice)
	: controllerDevice(controllerDevice)
{
}
