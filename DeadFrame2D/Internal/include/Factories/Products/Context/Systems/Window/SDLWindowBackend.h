#pragma once
#include "Core/Context/Systems/Window/Abstractions/IWindowBackend.h"
#include "DF2D_API.h"


struct SDL_Window;


namespace DF2D::Core
{
	class DF2D_API SDLWindowBackend : public IWindowBackend
	{
	private:
		SDL_Window* window = nullptr;


	public:
		SDLWindowBackend(const std::string& title, int width, int height, bool fullscreen);

		~SDLWindowBackend() override;

		SDLWindowBackend(const SDLWindowBackend&) = delete;

		SDLWindowBackend(SDLWindowBackend&&) = delete;

		SDLWindowBackend& operator=(const SDLWindowBackend&) = delete;

		SDLWindowBackend& operator=(SDLWindowBackend&&) = delete;


		void* GetNativeHandle() override;

		Core::Vector2I GetResolution() override;

		void SetWindowTitle(const std::string& title) override;

		void SetWindowIcon(std::string_view iconSource) override;

		void SetWindowMode(int mode) override;

		bool SetResolution(Core::Vector2I resolution) override;
	};
}