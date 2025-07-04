#include "SubSystems/Renderer.h"
#include "SubSystems/TextureManager.h"
#include <Debugging/Debug.h>
#include <Math/Vector2.h>
#include <memory>


std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> TextureManager::textureCache = {};


TextureManager::TextureManager()
{
	auto initFlags = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	if ((initFlags & (IMG_INIT_PNG | IMG_INIT_JPG)) != (IMG_INIT_PNG | IMG_INIT_JPG))
	{
		std::cerr << "Failed to initialize SDL_image with PNG/JPG support! Error: " << IMG_GetError() << std::endl;

		return;
	}

	std::cout << "[Info] SDL_image successfully initialized." << std::endl;
}

TextureManager::~TextureManager()
{
	textureCache.clear();

	IMG_Quit();

	std::cout << "[Info] SDL_image subsystem successfully quit." << std::endl;
}

void TextureManager::Update(float deltaTime)
{

}

void TextureManager::BeginFrame()
{

}

void TextureManager::EndUpdate()
{

}

void TextureManager::EndDraw()
{

}

std::shared_ptr<SDL_Texture> TextureManager::LoadTexture(std::string_view filename)
{
	auto filenameString = std::string(filename);

	auto it = textureCache.find(filenameString);

	if (it != textureCache.end())
	{
		if (auto sharedPtr = it->second.lock())
			return sharedPtr;
	}

	auto tempSurface = IMG_Load(filenameString.c_str());

#if _DEBUG
	DBG_ASSERT_MSG(tempSurface, "Failed to load the surface: % s\n", SDL_GetError());
#endif

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetRenderer(), tempSurface);

#if _DEBUG
	DBG_ASSERT_MSG(texture, "Failed to create the texture: %s\n", SDL_GetError());
#endif

	SDL_FreeSurface(tempSurface);
	tempSurface = nullptr;

	auto sharedPtr = std::shared_ptr<SDL_Texture>(texture, [filenameString](SDL_Texture* texture)
		{
			SDL_DestroyTexture(texture);

			TextureManager::textureCache.erase(filenameString);
		});

	textureCache[filenameString] = sharedPtr;

	return sharedPtr;
}

void TextureManager::DrawRect(SDL_Rect rect, float angleDegrees, SDL_Color color, bool filled)
{
	auto renderer = Renderer::GetRenderer();
	auto oldRenderColor = Renderer::GetDisplayColor();

	Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

	auto cx = rect.x + rect.w * 0.5f;
	auto cy = rect.y + rect.h * 0.5f;

	auto angleRad = angleDegrees * (MathConstants::PI / 180.0f);
	auto cosA = std::cos(angleRad);
	auto sinA = std::sin(angleRad);

	SDL_FPoint corners[4] = 
	{
		{ -rect.w / 2.0f, -rect.h / 2.0f },
		{  rect.w / 2.0f, -rect.h / 2.0f },
		{  rect.w / 2.0f,  rect.h / 2.0f },
		{ -rect.w / 2.0f,  rect.h / 2.0f }
	};

	for (auto& p : corners)
	{
		auto x = p.x * cosA - p.y * sinA;
		auto y = p.x * sinA + p.y * cosA;

		p.x = x + cx;
		p.y = y + cy;
	}

	if (filled)
	{
		SDL_Vertex vertices[6];

		SDL_Color vertexColor = color;

		vertices[0] = { corners[0], vertexColor, {0, 0} };
		vertices[1] = { corners[1], vertexColor, {0, 0} };
		vertices[2] = { corners[2], vertexColor, {0, 0} };

		vertices[3] = { corners[2], vertexColor, {0, 0} };
		vertices[4] = { corners[3], vertexColor, {0, 0} };
		vertices[5] = { corners[0], vertexColor, {0, 0} };

		SDL_RenderGeometry(renderer, nullptr, vertices, 6, nullptr, 0);
	}
	else
	{
		for (auto i = 0; i < 4; ++i)
		{
			SDL_RenderDrawLineF(
				renderer,
				corners[i].x, corners[i].y,
				corners[(i + 1) % 4].x, corners[(i + 1) % 4].y);
		}
	}

	Renderer::SetDisplayColor(oldRenderColor.r, oldRenderColor.g, oldRenderColor.b, oldRenderColor.a);
}

void TextureManager::DrawCircle(Circle circle, SDL_Color color, bool filled)
{
	auto radius = circle.radius;
	auto position = circle.position;

	auto oldRenderColor = Renderer::GetDisplayColor();
	
	Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

	SDL_Renderer* renderer = Renderer::GetRenderer();
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	const int diameter = (radius * 2);

	int x = radius - 1;
	int y = 0;
	int tx = 1;
	int ty = 1;
	int error = (tx - diameter);

	while (x >= y)
	{
		if (filled)
		{
			// Draw horizontal lines between points to fill the circle
			SDL_RenderDrawLine(renderer, position.x - x, position.y - y, position.x + x, position.y - y);
			SDL_RenderDrawLine(renderer, position.x - x, position.y + y, position.x + x, position.y + y);
			SDL_RenderDrawLine(renderer, position.x - y, position.y - x, position.x + y, position.y - x);
			SDL_RenderDrawLine(renderer, position.x - y, position.y + x, position.x + y, position.y + x);
		}
		else
		{
			// Draw outline points
			SDL_RenderDrawPoint(renderer, position.x + x, position.y - y);
			SDL_RenderDrawPoint(renderer, position.x + x, position.y + y);
			SDL_RenderDrawPoint(renderer, position.x - x, position.y - y);
			SDL_RenderDrawPoint(renderer, position.x - x, position.y + y);
			SDL_RenderDrawPoint(renderer, position.x + y, position.y - x);
			SDL_RenderDrawPoint(renderer, position.x + y, position.y + x);
			SDL_RenderDrawPoint(renderer, position.x - y, position.y - x);
			SDL_RenderDrawPoint(renderer, position.x - y, position.y + x);
		}

		if (error <= 0)
		{
			y++;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			x--;
			tx += 2;
			error += (tx - diameter);
		}
	}

	// Reset Display Color
	Renderer::SetDisplayColor(oldRenderColor.r, oldRenderColor.g, oldRenderColor.b, oldRenderColor.a);
}

void TextureManager::DrawTexture(
	std::shared_ptr<SDL_Texture> texture,
	const SDL_Rect* srcRect, 
	const SDL_Rect* dstRect, 
	float angle,
	SDL_Point* rotationOrigin, 
	SDL_RendererFlip flip, 
	Uint8 alpha, 
	SDL_Color colorMod)
{
	auto renderer = Renderer::GetRenderer();

	if (renderer == nullptr || texture == nullptr)
		return;

	auto texturePtr = texture.get();

	if (dstRect == NULL)
	{
		auto localDst = SDL_Rect{ 0, 0, 0, 0 };
		
		SDL_QueryTexture(texturePtr, nullptr, nullptr, &localDst.w, &localDst.h);

		dstRect = &localDst;
	}

	// Backup current texture state
	Uint8 oldAlpha, oldR, oldG, oldB;
	SDL_GetTextureAlphaMod(texturePtr, &oldAlpha);
	SDL_GetTextureColorMod(texturePtr, &oldR, &oldG, &oldB);

	// Apply new modulation
	SDL_SetTextureAlphaMod(texturePtr, alpha);
	SDL_SetTextureColorMod(texturePtr, colorMod.r, colorMod.g, colorMod.b);

	// Default to center if NULL
	auto rotationOriginFallback = SDL_Point
	{
		static_cast<int>(dstRect->w * 0.5f),
		static_cast<int>(dstRect->h * 0.5f)
	};

	if (rotationOrigin == NULL)
	{
		rotationOrigin = &rotationOriginFallback;
	}

	// Render
	SDL_RenderCopyEx(renderer, texturePtr, srcRect, dstRect, angle, rotationOrigin, flip);

	// Restore previous modulation
	SDL_SetTextureAlphaMod(texturePtr, oldAlpha);
	SDL_SetTextureColorMod(texturePtr, oldR, oldG, oldB);
}