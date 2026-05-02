#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Math/Vector2.h"
#include "Core/Services/Events/Abstractions/IEventProcessor.h"
#include "Data/Systems/Window/WindowMode.h"
#include "DF2D_API.h"
#include "Models/Window/WindowConfig.h"
#include <SDL_video.h>
#include <vector>


namespace DF2D::Core
{
	class DF2D_API Window : public IEventProcessor, public ICoreSystem
	{
		friend class SystemInitializer;


	private:
		static SDL_Window* window;


		Window(Models::WindowConfig windowConfig);

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

		static void SetWindowMode(Data::WindowMode mode);

		static bool SetResolution(Vector2I resolution);
	};
}