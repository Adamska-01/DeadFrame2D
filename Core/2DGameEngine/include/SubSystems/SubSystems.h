#pragma once


class Window;
class Renderer;
class Input;
class UIManager;
class TextureManager;
class AudioManager;


class SubSystems
{
private:
	Window* window;

	Renderer* renderer;

	Input* input;

	UIManager* uiManager;

	TextureManager* textureManager;

	AudioManager* audioManager;


public:
	SubSystems();
	
	~SubSystems();


	void InitializeSubSystems();


	void BeginFrame();
};