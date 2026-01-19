#pragma once
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "Data/Window/WindowMode.h"
#include "DF2D_API.h"
#include <Models/Window/WindowConfig.h>
#include <vector>


namespace DeadFrame2D::Core
{
	class DF2D_API Window : public IEventProcessor, public ISubSystem
	{
		friend class SubSystemManager;


	private:
		static SDL_Window* window;


		Window(Shared::Models::WindowConfig windowConfig);

		~Window() override;

		Window(const Window&) = delete;

		Window(Window&&) = delete;


		Window& operator=(const Window&) = delete;

		Window& operator=(Window&&) = delete;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	public:
		static SDL_Window* GetWindow();

		static Vector2I GetResolution();

		static std::vector<SDL_DisplayMode> GetSupportedResolutions();

		static void SetWindowTitle(const std::string& title);

		static void SetWindowIcon(std::string_view iconSource);

		static void SetWindowMode(DeadFrame2D::Data::WindowMode mode);

		static bool SetResolution(Vector2I resolution);
	};
}