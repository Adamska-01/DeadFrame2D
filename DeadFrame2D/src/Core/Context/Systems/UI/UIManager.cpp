#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Core/Debugging/Debug.h"
#include <iostream>
#include <sstream>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Utilities;


	std::unordered_map<std::pair<std::string, int>, std::shared_ptr<TTF_Font>, PairHash> UIManager::fontCache = {};


	UIManager::UIManager()
	{
		if (TTF_Init() < 0)
		{
			std::cerr << "Failed to initialize SDL_TTF: " << SDL_GetError() << std::endl;

			return;
		}

		std::cout << "[Info] SDL_TTF successfully initialized." << std::endl;
	}

	UIManager::~UIManager()
	{
		fontCache.clear();

		TTF_Quit();

		std::cout << "[Info] SDL_TTF subsystem successfully quit." << std::endl;
	}

	void UIManager::BeginFrame()
	{

	}

	void UIManager::PreUpdate(float deltaTime)
	{

	}

	void UIManager::EndUpdate(float deltaTime)
	{

	}

	void UIManager::EndDraw()
	{

	}

	std::shared_ptr<TTF_Font> UIManager::LoadFont(std::string_view textSource, int fontsize)
	{
		auto filenameString = std::string(textSource);

		// Check if the font is already cached
		auto key = std::make_pair(filenameString, fontsize);
		auto it = fontCache.find(key);
	
		if (it != fontCache.end()) 
			return it->second;

		// If the font is not in the cache, load it from the file
		auto font = std::shared_ptr<TTF_Font>(TTF_OpenFont(filenameString.c_str(), fontsize), TTF_CloseFont);

		// Check if the font loaded successfully
	#if _DEBUG
		DBG_ASSERT_MSG(font, "Failed to load the font: %s\n", TTF_GetError());
	#endif

		// Store the font in the cache (shared_ptr automatically manages the memory)
		fontCache[key] = font;

		return font;
	}

	std::shared_ptr<SDL_Texture> UIManager::LoadText(std::shared_ptr<TTF_Font> font, std::string text, SDL_Color color, bool centerText)
	{
		if (text.empty())
			return nullptr;

		// Helper: Expand tab characters to spaces
		auto ExpandTabs = [](const std::string& line, int tabWidth = 4) -> std::string
			{
				std::string result;
				auto col = 0;

				for (auto ch : line)
				{
					if (ch == '\t')
					{
						auto spaces = tabWidth - (col % tabWidth);
						result.append(spaces, ' ');
						col += spaces;
					}
					else
					{
						result += ch;
						++col;
					}
				}

				return result;
			};

		// Split into lines (preserve blank lines and trailing newlines)
		std::vector<std::string> lines;
		std::istringstream stream(text);
		std::string line;

		while (std::getline(stream, line))
		{
			if (line.empty())
			{
				line = " ";
			}

			lines.push_back(line);
		}


		// Render each line to get total width and height
		std::vector<SDL_Surface*> lineSurfaces;
		auto maxWidth = 0;
		auto totalHeight = 0;

		for (const auto& rawLine : lines)
		{
			auto expanded = ExpandTabs(rawLine); // expand tabs

			auto surface = TTF_RenderText_Blended(font.get(), expanded.c_str(), color);

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
			return nullptr;

		// Create final surface
		auto finalSurface = SDL_CreateRGBSurfaceWithFormat(0, maxWidth, totalHeight, 32, SDL_PIXELFORMAT_RGBA32);
		SDL_FillRect(finalSurface, nullptr, SDL_MapRGBA(finalSurface->format, 0, 0, 0, 0)); // transparent

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

		auto textTexture = SDL_CreateTextureFromSurface(Renderer::GetRenderer(), finalSurface);
		SDL_FreeSurface(finalSurface);

		if (!textTexture)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture: %s", TTF_GetError());

			return nullptr;
		}

		return std::shared_ptr<SDL_Texture>(textTexture, SDL_DestroyTexture);
	}
}