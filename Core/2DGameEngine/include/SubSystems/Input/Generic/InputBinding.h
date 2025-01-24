#pragma once
#include "InputDeviceType.h"
#include <SDL.h>
#include <string>


struct InputBinding
{
	InputDeviceType inputDeviceType;

	Uint8 inputKey;
};