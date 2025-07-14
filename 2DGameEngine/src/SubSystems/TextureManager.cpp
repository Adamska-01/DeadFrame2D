#include "Components/Rendering/Camera.h"
#include "SubSystems/Renderer.h"
#include "SubSystems/TextureManager.h"
#include <Debugging/Debug.h>
#include <Math/Vector2.h>
#include <memory>


std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> TextureManager::textureCache = {};

Camera* TextureManager::currentCamera = nullptr;


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

void TextureManager::DrawLine(const Vector2F& p1, const Vector2F& p2, SDL_Color color, Camera* camera)
{
	auto renderer = Renderer::GetRenderer();
	
	if (!renderer)
		return;
	
	auto screenP1 = p1, screenP2 = p2;

	if (camera != nullptr)
	{
		screenP1 = camera->WorldToScreen(p1);
		screenP2 = camera->WorldToScreen(p2);
	}

	auto oldRenderColor = Renderer::GetDisplayColor();
	
	Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

	SDL_RenderDrawLineF(
		renderer,
		screenP1.x, screenP1.y,
		screenP2.x, screenP2.y);

	Renderer::SetDisplayColor(oldRenderColor.r, oldRenderColor.g, oldRenderColor.b, oldRenderColor.a);
}

void TextureManager::DrawRect(SDL_Rect rect, float angleDegrees, SDL_Color color, bool filled, Camera* camera)
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

		if (camera != nullptr)
		{
			auto pVec = Vector2F(p.x, p.y);

			pVec = camera->WorldToScreen(pVec);

			p.x = pVec.x;
			p.y = pVec.y;
		}
	}

	if (filled)
	{
		SDL_Vertex vertices[6]
		{
			{ corners[0], color, {0, 0} },
			{ corners[1], color, {0, 0} },
			{ corners[2], color, {0, 0} },
			{ corners[2], color, {0, 0} },
			{ corners[3], color, {0, 0} },
			{ corners[0], color, {0, 0} }
		};

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

void TextureManager::DrawCircle(Circle circle, SDL_Color color, bool filled, Camera* camera)
{
	auto renderer = Renderer::GetRenderer();
	auto oldRenderColor = Renderer::GetDisplayColor();

	Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

	auto screenCenter = circle.position;
	auto radius = static_cast<int>(circle.radius);
	
	if (camera != nullptr)
	{
		screenCenter = camera->WorldToScreen(circle.position);
		radius *= camera->GetZoom();
	}

	auto x = radius - 1;
	auto y = 0;
	auto tx = 1;
	auto ty = 1;
	auto error = tx - (radius << 1);

	const int cx = static_cast<int>(screenCenter.x);
	const int cy = static_cast<int>(screenCenter.y);

	while (x >= y)
	{
		if (filled)
		{
			SDL_RenderDrawLine(renderer, cx - x, cy - y, cx + x, cy - y);
			SDL_RenderDrawLine(renderer, cx - x, cy + y, cx + x, cy + y);
			SDL_RenderDrawLine(renderer, cx - y, cy - x, cx + y, cy - x);
			SDL_RenderDrawLine(renderer, cx - y, cy + x, cx + y, cy + x);
		}
		else
		{
			SDL_RenderDrawPoint(renderer, cx + x, cy - y);
			SDL_RenderDrawPoint(renderer, cx + x, cy + y);
			SDL_RenderDrawPoint(renderer, cx - x, cy - y);
			SDL_RenderDrawPoint(renderer, cx - x, cy + y);
			SDL_RenderDrawPoint(renderer, cx + y, cy - x);
			SDL_RenderDrawPoint(renderer, cx + y, cy + x);
			SDL_RenderDrawPoint(renderer, cx - y, cy - x);
			SDL_RenderDrawPoint(renderer, cx - y, cy + x);
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
			error += (tx - (radius << 1));
		}
	}

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
	SDL_Color colorMod, 
	Camera* camera)
{
	auto renderer = Renderer::GetRenderer();

	if (renderer == nullptr || texture == nullptr)
		return;

	auto texturePtr = texture.get();

	auto floatPos = Vector2F::Zero;
	auto floatSize = Vector2F::Zero;

	if (!dstRect)
	{
		auto texW = 0, texH = 0;
		SDL_QueryTexture(texturePtr, nullptr, nullptr, &texW, &texH);

		floatSize = { static_cast<float>(texW), static_cast<float>(texH) };
	}
	else
	{
		floatPos = { static_cast<float>(dstRect->x), static_cast<float>(dstRect->y) };
		floatSize = { static_cast<float>(dstRect->w), static_cast<float>(dstRect->h) };
	}

	if (camera != nullptr)
	{
		// Transform position to screen space
		floatPos = camera->WorldToScreen(floatPos);

		// Apply zoom to size
		floatSize *= camera->GetZoom();
	}

	SDL_Rect transformedDst
	{
		static_cast<int>(std::floor(floatPos.x)),
		static_cast<int>(std::floor(floatPos.y)),
		static_cast<int>(std::ceil(floatSize.x)),
		static_cast<int>(std::ceil(floatSize.y))
	};

	// Backup texture state
	Uint8 oldAlpha, oldR, oldG, oldB;
	SDL_GetTextureAlphaMod(texturePtr, &oldAlpha);
	SDL_GetTextureColorMod(texturePtr, &oldR, &oldG, &oldB);

	SDL_SetTextureAlphaMod(texturePtr, alpha);
	SDL_SetTextureColorMod(texturePtr, colorMod.r, colorMod.g, colorMod.b);

	SDL_Point fallbackOrigin
	{
		static_cast<int>(std::roundf(floatSize.x * 0.5f)),
		static_cast<int>(std::roundf(floatSize.y * 0.5f))
	};

	SDL_RenderCopyEx(
		renderer, 
		texturePtr, 
		srcRect, 
		&transformedDst, 
		angle,
		rotationOrigin ? rotationOrigin : &fallbackOrigin, 
		flip);

	// Restore texture state
	SDL_SetTextureAlphaMod(texturePtr, oldAlpha);
	SDL_SetTextureColorMod(texturePtr, oldR, oldG, oldB);
}

void TextureManager::DrawLineWorldSpace(const Vector2F& p1, const Vector2F& p2, SDL_Color color)
{
	DrawLine(p1, p2, color, currentCamera);
}

void TextureManager::DrawLineScreenSpace(const Vector2F& p1, const Vector2F& p2, SDL_Color color)
{
	DrawLine(p1, p2, color);
}

void TextureManager::DrawRectWorldSpace(SDL_Rect rect, float angleDegrees, SDL_Color color, bool filled)
{
	DrawRect(rect, angleDegrees, color, filled, currentCamera);
}

void TextureManager::DrawRectScreenSpace(SDL_Rect rect, float angleDegrees, SDL_Color color, bool filled)
{
	DrawRect(rect, angleDegrees, color, filled);
}

void TextureManager::DrawCircleWorldSpace(Circle circle, SDL_Color color, bool filled)
{
	DrawCircle(circle, color, filled, currentCamera);
}

void TextureManager::DrawCircleScreenSpace(Circle circle, SDL_Color color, bool filled)
{
	DrawCircle(circle, color, filled);
}

void TextureManager::DrawTextureWorldSpace(
	std::shared_ptr<SDL_Texture> texture, 
	const SDL_Rect* srcRect, 
	const SDL_Rect* dstRect, 
	float angle, 
	SDL_Point* rotationOrigin, 
	SDL_RendererFlip flip, 
	Uint8 alpha, 
	SDL_Color colorMod)
{
	DrawTexture(texture, srcRect, dstRect, angle, rotationOrigin, flip, alpha, colorMod, currentCamera);
}

void TextureManager::DrawTextureScreenSpace(
	std::shared_ptr<SDL_Texture> texture, 
	const SDL_Rect* srcRect, 
	const SDL_Rect* dstRect, 
	float angle, 
	SDL_Point* rotationOrigin, 
	SDL_RendererFlip flip, 
	Uint8 alpha, 
	SDL_Color colorMod)
{
	DrawTexture(texture, srcRect, dstRect, angle, rotationOrigin, flip, alpha, colorMod);
}