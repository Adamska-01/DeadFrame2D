#include "SubSystems/Renderer.h"
#include "SubSystems/TextureManager.h"
#include <Debugging/Debug.h>
#include <fstream>
#include <Math/Vector2.h>
#include <SDL_ttf.h>
#include <sstream>


TextureManager* TextureManager::Instance = nullptr;


TextureManager::TextureManager()
{
	auto initFlags = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	if ((initFlags & (IMG_INIT_PNG | IMG_INIT_JPG)) != (IMG_INIT_PNG | IMG_INIT_JPG)) 
	{
		std::cerr << "Failed to initialize SDL_image with PNG/JPG support! Error: " << IMG_GetError() << std::endl;
	}
};

SDL_Texture* TextureManager::LoadTexture(std::string filename)
{
	auto tempSurface = IMG_Load(filename.c_str());

#if _DEBUG
	DBG_ASSERT_MSG(tempSurface, "Failed to load the surface: % s\n", SDL_GetError());
#endif

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetRenderer(), tempSurface);
		
#if _DEBUG
	DBG_ASSERT_MSG(texture, "Failed to create the texture: %s\n", SDL_GetError());
#endif

	SDL_FreeSurface(tempSurface);
	tempSurface = nullptr;

	return texture;
}

void TextureManager::NormalDraw(SDL_Texture* texture)
{
	SDL_RenderCopy(Renderer::GetInstance()->GetRenderer(), texture, NULL, NULL);
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect dest, Vector2 scale, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = { 0, 0, dest.w, dest.h };

	SDL_Rect destRect = { dest.x, dest.y, dest.w * scale.x, dest.h * scale.y };

	SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), texture, &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::DrawPortion(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, Vector2 scale, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = { src.x, src.y, src.w, src.h };

	SDL_Rect destRect = { 0, 0, dest.w * scale.x, dest.h * scale.y };

	SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), texture, &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(SDL_Texture* texture, SDL_Rect dest, Vector2 scale, int row, int frame, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = { dest.w * frame, dest.h * row, dest.w, dest.h };

	SDL_Rect destRect = { dest.x, dest.y, dest.w * scale.x, dest.h * scale.y };

	SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), texture, &srcRect, &destRect, 0, NULL, flip);
}

void TextureManager::DrawTile(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, int row, int frame, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), texture, &src, &dest, 0, nullptr, flip);
}

void TextureManager::DrawRotate(SDL_Texture* texture, Vector2 pos, Vector2 scale, float angle, SDL_Point rotPoint, SDL_RendererFlip flip)
{
	SDL_Rect destRect{ pos.x, pos.y, 0, 0 };

	SDL_QueryTexture(texture, NULL, NULL, &destRect.w, &destRect.h);

	destRect.w *= scale.x;
	destRect.h *= scale.y;

	SDL_RenderCopyEx(Renderer::GetInstance()->GetRenderer(), texture, NULL, &destRect, angle, &rotPoint, flip);
}

void TextureManager::Clean()
{
	IMG_Quit();
}