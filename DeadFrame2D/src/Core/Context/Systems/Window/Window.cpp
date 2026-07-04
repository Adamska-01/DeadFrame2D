#include "Core/Context/Systems/Window/Window.h"
#include <SDL.h>
#include <iostream>


namespace DF2D::Core
{
	using namespace DF2D::Data;


	Window::Window(std::unique_ptr<IWindowBackend> backend)
		: backend(std::move(backend))
	{
		std::cout << "[Info] Window successfully initialized." << std::endl;
	}

	Window::~Window()
	{
		backend.reset();
	}

	void Window::BeginFrame()
	{

	}

	void Window::PreUpdate(float deltaTime)
	{

	}

	void Window::EndUpdate(float deltaTime)
	{

	}

	void Window::EndDraw()
	{

	}

	std::optional<int> Window::ProcessEvents(const SDL_Event& sdlEvent)
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			return (int)SDL_QUIT;

		case SDL_WINDOWEVENT:
			if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
				return (int)SDL_QUIT;
			break;

		default:
			break;
		}

		return std::nullopt;
	}

	Vector2I Window::GetResolution()
	{
		return backend->GetResolution();
	}

	void Window::SetWindowTitle(const std::string& title)
	{
		backend->SetWindowTitle(title);
	}

	void Window::SetWindowIcon(std::string_view iconSource)
	{
		backend->SetWindowIcon(iconSource);
	}

	void Window::SetWindowMode(Data::WindowMode mode)
	{
		backend->SetWindowMode(mode);
	}

	bool Window::SetResolution(Vector2I resolution)
	{
		return backend->SetResolution(resolution);
	}
}
