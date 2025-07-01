#pragma once
#include "Generic/WindowMode.h"
#include "Math/Vector2.h"
#include "SubSystems/Abstractions/ISubSystem.h"
#include "SubSystems/Events/Interfaces/IEventProcessor.h"
#include <Models/Engine/WindowConfig.h>
#include <string>
#include <vector>


class Window : public IEventProcessor, public ISubSystem
{
	friend class SubSystems;


private:
	Window(WindowConfig windowConfig);

	~Window();

	Window(const Window&) = delete;

	Window(Window&&) = delete;


	Window& operator=(const Window&) = delete;

	Window& operator=(Window&&) = delete;


	static SDL_Window* window;


	virtual void Update(float deltaTime) override;

	virtual void BeginFrame() override;

	virtual void EndUpdate() override;

	virtual void EndDraw() override;


public:
	std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	static SDL_Window* GetWindow();

	static Vector2I GetResolution();

	static std::vector<SDL_DisplayMode> GetSupportedResolutions();

	static void SetWindowIcon(std::string_view iconSource);

	static void SetWindowMode(WindowMode mode);

	static bool SetResolution(Vector2I resolution);
};