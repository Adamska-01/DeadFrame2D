#pragma once


class Window;
class Renderer;
class Input;


class SubSystems
{
private:
	static Window* window;

	static Renderer* renderer;

	static Input* input;


public:
	SubSystems() = default;
	
	~SubSystems();


	void InitializeSubSystems();
};