#pragma once
#include "Window.h"
#include <cstdint>


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

	static SDL_Color GetDisplayColor();

	static Vector2I GetResolutionTarget();

	static void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	static void SetResolutionTarget(Vector2I targetResolution);
};