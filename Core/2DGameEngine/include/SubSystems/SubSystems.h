#pragma once


class Window;
class Renderer;
class Input;
class UIManager;
class TextureManager;
class AudioManager;
class PhysicsEngine2D;
class CoroutineScheduler;


class SubSystems
{
private:
	// TODO: Create and interface to store all these
	Window* window;

	Renderer* renderer;

	Input* input;

	UIManager* uiManager;

	TextureManager* textureManager;

	AudioManager* audioManager;
	
	PhysicsEngine2D* physicsEngine2D;

	CoroutineScheduler* coroutineScheduler;


public:
	SubSystems();
	
	~SubSystems();


	void InitializeSubSystems();


	void Update(float deltaTime);
	
	void BeginFrame();

	void EndUpdate();

	void EndDraw();
};