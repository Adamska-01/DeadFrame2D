#pragma once
#include <map>
#include <Math/Vector2.h>
#include <SDL_image.h>
#include <string>


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


public:
	static SDL_Texture* LoadTexture(std::string filename);

	static void NormalDraw(SDL_Texture* texture);
	
	static void Draw(SDL_Texture* texture, SDL_Rect dest, Vector2F scale, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	static void DrawPortion(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, Vector2F scale, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	static void DrawFrame(SDL_Texture* texture, SDL_Rect dest, Vector2F scale, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	static void DrawTile(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	static void DrawRotate(SDL_Texture* texture, Vector2F pos, Vector2F scale, float angle, SDL_Point rotPoint, SDL_RendererFlip flip = SDL_FLIP_NONE);
};