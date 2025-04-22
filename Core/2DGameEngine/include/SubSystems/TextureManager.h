#pragma once
#include "Components/Generic/Circle.h"
#include <memory>
#include <SDL_image.h>
#include <string>
#include <unordered_map>


class TextureManager
{
	friend class SubSystems;


private:
	TextureManager();

	~TextureManager();

	TextureManager(const TextureManager&) = delete;

	TextureManager(TextureManager&&) = delete;


	TextureManager& operator=(const TextureManager&) = delete;

	TextureManager& operator=(TextureManager&&) = delete;


	static std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textureCache;


public:
	static std::shared_ptr<SDL_Texture> LoadTexture(std::string_view filename);


	static void DrawRect(SDL_Rect rect, SDL_Color color, bool filled);

	static void DrawCircle(Circle circle, SDL_Color color, bool filled);

	static void DrawTexture(
		std::shared_ptr<SDL_Texture> texture,
		const SDL_Rect* srcRect = NULL, 
		const SDL_Rect* dstRect = NULL, 
		float angle = 0.0f, 
		SDL_Point* rotationOrigin = NULL,
		SDL_RendererFlip flip = SDL_FLIP_NONE, 
		Uint8 alpha = 255, 
		SDL_Color colorMod = { 255, 255, 255, 255 });
};