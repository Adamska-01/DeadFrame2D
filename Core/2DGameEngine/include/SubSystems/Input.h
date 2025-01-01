#pragma once
#include "SubSystems//Events/Interfaces/IEventProcessor.h"
#include <unordered_map>


enum InputDeviceFlag
{
	DEVICE_KEYBOARD = 1 << 0,

	DEVICE_MOUSE = 1 << 1,
	
	DEVICE_CONTROLLER_1 = 1 << 2,
	
	DEVICE_CONTROLLER_2 = 1 << 3,
	
	DEVICE_CONTROLLER_3 = 1 << 4,
	
	DEVICE_CONTROLLER_4 = 1 << 5
};


struct InputState
{
	std::unordered_map<int, bool> buttons;

	std::unordered_map<int, float> axes;
};


class Input : public IEventProcessor
{
private:
	static std::unordered_map<InputDeviceFlag, InputState> currentInputs;

	static std::unordered_map<InputDeviceFlag, InputState> lastInputs;

	static std::vector<SDL_GameController*> controllers;


	static void InitialiseControllers();

	static void ProcessControllerEvents(const SDL_Event& controllerEvent);

	static void ProcessKeyboardEvents(const SDL_Event& keyboardEvent);

	static void ProcessMouseEvents(const SDL_Event& mouseEvent);

	static bool IsDeviceValid(InputDeviceFlag deviceFlag);


	void Cleanup();


public:
	Input();

	~Input();


	std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	/******************************************************************************
	 * Takes:
	 * - SDL_Scancode for keyboard input.<br>\n
	 * - SDL_GameControllerButton for controller input.
	 * - Look at 'SDL_mouse.h' for mouse input. (it's just a bunch of macros)
	 ******************************************************************************/
	static bool IsButtonPressed(InputDeviceFlag deviceFlag, int button);

	/******************************************************************************
	 * Takes:
	 * - SDL_Scancode for keyboard input.<br>\n
	 * - SDL_GameControllerButton for controller input.
	 * - Look at 'SDL_mouse.h' for mouse input. (it's just a bunch of macros)
	 ******************************************************************************/
	static	bool IsButtonHeld(InputDeviceFlag deviceFlag, int button);

	/******************************************************************************
	 * Takes:
	 * - SDL_Scancode for keyboard input.<br>\n
	 * - SDL_GameControllerButton for controller input.
	 * - Look at 'SDL_mouse.h' for mouse input. (it's just a bunch of macros)
	 ******************************************************************************/
	static float GetAxisValue(InputDeviceFlag deviceFlag, int axis);

	static bool IsControllerConnected(InputDeviceFlag deviceFlag);
};
