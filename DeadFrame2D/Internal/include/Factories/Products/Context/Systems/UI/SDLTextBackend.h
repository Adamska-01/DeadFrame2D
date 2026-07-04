#pragma once
#include "Core/Context/Systems/UI/Abstractions/ITextBackend.h"
#include "Data/TextureRegistry.h"
#include "DF2D_API.h"
#include <memory>
#include <unordered_map>


struct SDL_Renderer;

typedef struct _TTF_Font TTF_Font;


namespace DF2D::Internal
{
	class DF2D_API SDLTextBackend : public Core::ITextBackend
	{
	private:
		SDL_Renderer* renderer = nullptr;

		TextureRegistry* registry = nullptr;

		std::unordered_map<Data::FontID, std::shared_ptr<TTF_Font>> fonts;

		Data::FontID nextFontID = 1;


	public:
		SDLTextBackend(SDL_Renderer* renderer, TextureRegistry* textureRegistry);

		~SDLTextBackend() override;

		SDLTextBackend(const SDLTextBackend&) = delete;

		SDLTextBackend(SDLTextBackend&&) = delete;

		SDLTextBackend& operator=(const SDLTextBackend&) = delete;

		SDLTextBackend& operator=(SDLTextBackend&&) = delete;


		Data::FontID LoadFont(const std::string& path, int size) override;

		void SetFontStyle(Data::FontID font, Data::FontStyle style) override;

		Data::TextTexture CreateTextTexture(Data::FontID font, const std::vector<std::string>& lines, Core::Color color, bool centerText) override;
	};
}