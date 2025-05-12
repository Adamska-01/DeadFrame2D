#pragma once


class Window;
class Renderer;
class Input;
class UIManager;
class TextureManager;
class AudioManager;
class PhysicsEngine2D;


class SubSystems
{
private:
	Window* window;

	Renderer* renderer;

	Input* input;

	UIManager* uiManager;

	TextureManager* textureManager;

	AudioManager* audioManager;
	
	PhysicsEngine2D* physicsEngine2D;


public:
	SubSystems();
	
	~SubSystems();


	void InitializeSubSystems();


	void BeginFrame();
};