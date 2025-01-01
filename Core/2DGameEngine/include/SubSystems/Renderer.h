#pragma once
#include "Window.h"


struct SDL_Window;
struct SDL_Renderer;


class Renderer
{
private:
	static SDL_Renderer* renderer;


public:
	Renderer(SDL_Window* window);

	~Renderer();

	
	static void ClearBuffer();
	
	static void PresentBuffer();
	
	static SDL_Renderer* GetRenderer();

	static void SetDisplayColour(int r, int g, int b, int a);

	static void SetResolution(int width, int height);
};