#pragma once
#include <map>
#include <Math/Vector2.h>
#include <SDL_image.h>
#include <string>


class TextureManager
{
private:
	TextureManager();


	static TextureManager* Instance;


public:
	inline static TextureManager* GetInstance() { return Instance = (Instance != nullptr) ? Instance : new TextureManager(); }


	SDL_Texture* LoadTexture(std::string filename);

	void NormalDraw(SDL_Texture* texture);
	
	void Draw(SDL_Texture* texture, SDL_Rect dest, Vector2 scale, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void DrawPortion(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, Vector2 scale, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void DrawFrame(SDL_Texture* texture, SDL_Rect dest, Vector2 scale, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void DrawTile(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void DrawRotate(SDL_Texture* texture, Vector2 pos, Vector2 scale, float angle, SDL_Point rotPoint, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void Clean();
};