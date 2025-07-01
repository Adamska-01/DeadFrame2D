#pragma once
#include "SubSystems/Abstractions/ISubSystem.h"
#include "Window.h"
#include <cstdint>
#include <Models/Engine/RendererConfig.h>


struct SDL_Window;
struct SDL_Renderer;


class Renderer : public ISubSystem
{
	friend class SubSystems;


private:
	Renderer(SDL_Window* window, RendererConfig config);

	~Renderer();

	Renderer(const Renderer&) = delete;

	Renderer(Renderer&&) = delete;


	Renderer& operator=(const Renderer&) = delete;

	Renderer& operator=(Renderer&&) = delete;


	static SDL_Renderer* renderer;


	virtual void Update(float deltaTime) override;

	virtual void BeginFrame() override;

	virtual void EndUpdate() override;

	virtual void EndDraw() override;


public:
	static void ClearBuffer();
	
	static void PresentBuffer();
	
	static SDL_Renderer* GetRenderer();

	static SDL_Color GetDisplayColor();

	static Vector2I GetResolutionTarget();

	static void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	static void SetResolutionTarget(Vector2I targetResolution);
};