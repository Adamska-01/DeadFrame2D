#include <SDL.h>
#include <SubSystems/Input.h>


Input* Input::Instance = nullptr;


Input::Input()
{
	padHolder = nullptr;

	Controllers.clear();
	CurrentControllerInputs.clear();
	LastControllerInputs.clear();
	
	numGamepads = 0;

	if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) == 0)
	{
		if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0)
		{
			std::cerr << "Error initializing SDL GameController subsystem: " << SDL_GetError() << std::endl;
		}

		std::cout << "SDL GameController subsystem initialized successfully." << std::endl;
	}
}

Input::~Input()
{
	Clean();
}

void Input::InitialiseController()
{
	auto nJoysticks = SDL_NumJoysticks();

	numGamepads = 0;

	// Count how many controllers there are
	for (int i = 0; i < nJoysticks; i++)
	{
		if (SDL_IsGameController(i))
		{
			numGamepads++;
		}
	}

	SDL_GameControllerEventState(SDL_ENABLE);

	// Vectors are empty to begin with, this sets their size
	Controllers.resize(numGamepads);
	CurrentControllerInputs.resize(numGamepads);
	LastControllerInputs.resize(numGamepads);

	// Set the status of the controllers to "nothing is happening"
	for (int i = 0; i < numGamepads; i++) 
	{
		for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; a++) 
		{
			CurrentControllerInputs[i].axis[a] = 0;
			LastControllerInputs[i].axis[a] = 0;
		}
		for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; b++) 
		{
			CurrentControllerInputs[i].buttons[b] = false;
			LastControllerInputs[i].buttons[b] = false;
		}
	}
}

bool Input::Update()
{
	//copies the button states from the previous frame into “previous” list.
	for (int i = 0; i < numGamepads; i++) 
	{
		for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; a++) 
		{
			LastControllerInputs[i].axis[a] = CurrentControllerInputs[i].axis[a];
		}
		for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; b++) {
			LastControllerInputs[i].buttons[b] = CurrentControllerInputs[i].buttons[b];
		}
	}

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type) 
		{
			case SDL_QUIT:
				return false;
				break;

				// This happens when a device is added
			case SDL_CONTROLLERDEVICEADDED:
#if _DEBUG
				std::cout << "DEVICE ADDED = " << event.cdevice.which << std::endl;
#endif
				padHolder = SDL_GameControllerOpen(event.cdevice.which);

				if (SDL_GameControllerGetAttached(padHolder) == 1)
				{
					Controllers.push_back(padHolder);
					for (auto i = 0; i < Controllers.size(); i++)
					{
						if (Controllers[i] == nullptr)
						{
							Controllers.erase(Controllers.begin() + i);
						}
					}
				}
#if _DEBUG
				else
				{
					fprintf(stderr, "Could not open controller: %s\n", SDL_GetError());
				}
#endif
				InitialiseController();
				break;

				// If a controller button is pressed
			case SDL_CONTROLLERBUTTONDOWN:
				// Cycle through the controllers
				for (auto i = 0; i < numGamepads; i++) 
				{
					// Looking for the button that was pressed
					if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(Controllers[i]))) 
					{
						// So the relevant state can be updated
						CurrentControllerInputs[i].buttons[event.cbutton.button] = true;
					}
				}
				break;

				// Do the same for releasing a button
			case SDL_CONTROLLERBUTTONUP:
				for (int i = 0; i < numGamepads; i++) 
				{
					if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(Controllers[i]))) 
					{
						CurrentControllerInputs[i].buttons[event.cbutton.button] = false;
					}
				}
				break;

				// And something similar for axis motion
			case SDL_CONTROLLERAXISMOTION:
				for (auto i = 0; i < numGamepads; i++) 
				{
					if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(Controllers[i]))) 
					{
						CurrentControllerInputs[i].axis[event.caxis.axis] = event.caxis.value;
					}
				}
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
			{
#if _DEBUG
				std::cout << "DEVICE REMOVED = " << event.cdevice.which << std::endl;
#endif
				SDL_Joystick* joystickToRemove = SDL_JoystickFromInstanceID(event.cdevice.which);

				// Loop thorugh the vector with the controllers
				for (int i = 0; i < Controllers.size(); i++)
				{
					if (SDL_GameControllerGetJoystick(Controllers[i]) == joystickToRemove)
					{
						SDL_GameControllerClose(Controllers[i]);
					}
				}

				InitialiseController();
			}
			break;
		}
	}

	return true;
}

bool Input::ButtonPressed(Players controllerID, SDL_GameControllerButton button)
{
	if (!IsControllerConnected(controllerID))
		return false;

	if (controllerID < 0 || controllerID > numGamepads)
		return false;

	return CurrentControllerInputs[controllerID].buttons[button] && !LastControllerInputs[controllerID].buttons[button];
}

bool Input::ButtonHeld(Players controllerID, SDL_GameControllerButton button)
{
	if (!IsControllerConnected(controllerID))
		return false;

	if (controllerID < 0 || controllerID > numGamepads)
		return false;

	return CurrentControllerInputs[controllerID].buttons[button] && LastControllerInputs[controllerID].buttons[button];
}

float Input::GetControllerAxis(Players controllerID, SDL_GameControllerAxis axis)
{
	if (!IsControllerConnected(controllerID))
		return 0.0;

	if (controllerID < 0 || controllerID > numGamepads)
		return 0.0;

	return CurrentControllerInputs[controllerID].axis[axis] / 32768.0f;
}

bool Input::IsControllerConnected(Players controllerID)
{
	if (Controllers.size() > controllerID && SDL_GameControllerGetAttached(Controllers[controllerID]) == 1)
		return true;

	return false;
}

void Input::Clean()
{
	auto controllerSize = Controllers.size();

	if (Controllers.size() <= 0)
		return;

	for (int i = 0; i < Controllers.size(); i++)
	{
		SDL_GameControllerClose(Controllers[i]);
		Controllers[i] = nullptr;
	}
}

int Input::GetControllersNumber() const
{ 
	return numGamepads; 
}