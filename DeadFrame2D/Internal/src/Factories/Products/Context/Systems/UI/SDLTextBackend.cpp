#include "Converters/Rendering/RenderingConversions.h"
#include "Factories/Products/Context/Systems/UI/SDLTextBackend.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Internal::RenderingConversions;


	SDLTextBackend::SDLTextBackend(SDL_Renderer* renderer, TextureRegistry* textureRegistry)
		: renderer(renderer),
		registry(textureRegistry)
	{
		if (TTF_Init() < 0)
		{
			std::cerr << "Failed to initialize SDL_TTF: " << TTF_GetError() << std::endl;

			return;
		}

		std::cout << "[Info] SDL_TTF successfully initialized." << std::endl;
	}

	SDLTextBackend::~SDLTextBackend()
	{
		fonts.clear();

		TTF_Quit();

		std::cout << "[Info] SDL_TTF subsystem successfully quit." << std::endl;
	}


	FontID SDLTextBackend::LoadFont(const std::string& path, int size)
	{
		auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(path.c_str(), size), TTF_CloseFont);

		if (font == nullptr)
		{
			std::cerr << "Failed to load font '" << path << "': " << TTF_GetError() << std::endl;

			return 0;
		}

		auto id = nextFontID++;

		fonts[id] = font;

		return id;
	}

	void SDLTextBackend::SetFontStyle(FontID font, FontStyle style)
	{
		auto it = fonts.find(font);

		if (it == fonts.end())
			return;

		TTF_SetFontStyle(it->second.get(), static_cast<int>(style));
	}

	TextTexture SDLTextBackend::CreateTextTexture(FontID font, const std::vector<std::string>& lines, Core::Color color, bool centerText)
	{
		auto it = fonts.find(font);

		if (it == fonts.end())
			return {};

		auto* ttfFont = it->second.get();
		auto sdlColor = ToSDLColor(color);

		std::vector<SDL_Surface*> lineSurfaces;
		auto maxWidth = 0;
		auto totalHeight = 0;

		for (const auto& line : lines)
		{
			auto surface = TTF_RenderText_Blended(ttfFont, line.c_str(), sdlColor);

			if (surface == nullptr)
			{
				SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render line: %s", TTF_GetError());
				continue;
			}

			maxWidth = std::max(maxWidth, surface->w);
			totalHeight += surface->h;
			lineSurfaces.push_back(surface);
		}

		if (lineSurfaces.empty())
			return {};

		auto finalSurface = SDL_CreateRGBSurfaceWithFormat(0, maxWidth, totalHeight, 32, SDL_PIXELFORMAT_RGBA32);
		SDL_FillRect(finalSurface, nullptr, SDL_MapRGBA(finalSurface->format, 0, 0, 0, 0));

		auto y = 0;
		for (auto lineSurface : lineSurfaces)
		{
			auto dstRect = SDL_Rect
			{
				.x = centerText ? (maxWidth - lineSurface->w) / 2 : 0,
				.y = y,
				.w = lineSurface->w,
				.h = lineSurface->h
			};

			SDL_BlitSurface(lineSurface, nullptr, finalSurface, &dstRect);

			y += lineSurface->h;

			SDL_FreeSurface(lineSurface);
		}

		auto texture = SDL_CreateTextureFromSurface(renderer, finalSurface);

		SDL_FreeSurface(finalSurface);

		if (texture == nullptr)
		{
			std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;

			return {};
		}

		return TextTexture
		{
			.textureID = registry->AddTexture(texture),
			.size = Vector2I(maxWidth, totalHeight)
		};
	}
}