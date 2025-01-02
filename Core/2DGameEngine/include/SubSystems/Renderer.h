#pragma once
#include "Window.h"


struct SDL_Window;
struct SDL_Renderer;


class Renderer
{
	friend class SubSystems;


private:
	Renderer(SDL_Window* window);

	~Renderer();

	Renderer(const Renderer&) = delete;

	Renderer(Renderer&&) = delete;


	Renderer& operator=(const Renderer&) = delete;

	Renderer& operator=(Renderer&&) = delete;


	static SDL_Renderer* renderer;


public:
	static void ClearBuffer();
	
	static void PresentBuffer();
	
	static SDL_Renderer* GetRenderer();

	static void SetDisplayColour(int r, int g, int b, int a);

	static void SetResolutionTarget(Vector2I targetResolution);
};