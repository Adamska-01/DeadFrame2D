#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Window/Abstractions/IWindowBackend.h"
#include "Core/Math/Vector2.h"
#include "Core/Services/Events/Abstractions/IEventProcessor.h"
#include "Data/Systems/Window/WindowMode.h"
#include "DF2D_API.h"
#include <memory>


union SDL_Event;


namespace DF2D::Core
{
	class DF2D_API Window : public IEventProcessor, public ICoreSystem
	{
	private:
		std::unique_ptr<IWindowBackend> backend;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;

		std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	public:
		Window(std::unique_ptr<IWindowBackend> backend);

		~Window() override;

		Window(const Window&) = delete;

		Window(Window&&) = delete;

		Window& operator=(const Window&) = delete;

		Window& operator=(Window&&) = delete;


		Vector2I GetResolution();

		void SetWindowTitle(const std::string& title);

		void SetWindowIcon(std::string_view iconSource);

		void SetWindowMode(Data::WindowMode mode);

		bool SetResolution(Vector2I resolution);
	};
}