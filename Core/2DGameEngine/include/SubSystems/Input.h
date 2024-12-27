#pragma once
#include <iostream>
#include <vector>
#include <SDL_gamecontroller.h>


enum Players
{
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4
};


struct  GamePad {
	bool buttons[SDL_CONTROLLER_BUTTON_MAX];
	int axis[SDL_CONTROLLER_AXIS_MAX];
};


class Input
{
private:
	Input();


	static Input* Instance;


	SDL_GameController* padHolder;

	std::vector<SDL_GameController*> Controllers{ nullptr };
	
	std::vector<GamePad> CurrentControllerInputs;
	
	std::vector<GamePad> LastControllerInputs;

	int numGamepads;


public:
	~Input();
	

	inline static Input* GetInstance() { return Instance = (Instance != nullptr) ? Instance : new Input(); }


	void InitialiseController();
	
	bool Update();
	
	bool ButtonPressed(Players controllerID, SDL_GameControllerButton button);
	
	bool ButtonHeld(Players controllerID, SDL_GameControllerButton button);
	
	float GetControllerAxis(Players controllerID, SDL_GameControllerAxis axis);

	bool IsControllerConnected(Players controllerID);

	void Clean();

	int GetControllersNumber() const;
};

//Built on sousage factory games code
//https://sausage-factory.games/dev-blog/easier-game-controller-input-in-sdl/