#pragma once
#include "InputDeviceType.h"
#include <SDL_stdinc.h>


struct InputBinding
{
	InputDeviceType inputDeviceType;

	Uint8 inputKey;
};