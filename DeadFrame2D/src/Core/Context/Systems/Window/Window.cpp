#include "Core/Context/Systems/Window/Window.h"
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