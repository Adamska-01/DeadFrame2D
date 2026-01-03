#include "Core/Debugging/Debug.h"
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/Renderer.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Engine/Components/Rendering/Camera.h"
#include <memory>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Constants;


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

	void TextureManager::BeginFrame()
	{

	}

	void TextureManager::PreUpdate(float deltaTime)
	{

	}

	void TextureManager::EndUpdate(float deltaTime)
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

	void TextureManager::DrawLineWorldSpace(const Vector2F& p1, const Vector2F& p2, SDL_Color color)
	{
		auto renderer = Renderer::GetRenderer();
		auto camera = Renderer::GetActiveCamera();

		if (renderer == nullptr || camera == nullptr || camera->GetRenderTarget() == nullptr)
			return;

		auto screenP1 = camera->WorldToScreen(p1);
		auto screenP2 = camera->WorldToScreen(p2);

		if (!camera->IsVisible(screenP1, screenP2)) 
			return;

		auto oldColor = Renderer::GetDisplayColor();
		Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

		SDL_RenderDrawLineF(renderer, screenP1.x, screenP1.y, screenP2.x, screenP2.y);

		Renderer::SetDisplayColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	}

	void TextureManager::DrawLineScreenSpace(const Vector2F& p1, const Vector2F& p2, SDL_Color color)
	{
		auto renderer = Renderer::GetRenderer();

		if (renderer == nullptr)
			return;

		auto oldColor = Renderer::GetDisplayColor();
		Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

		SDL_RenderDrawLineF(renderer, p1.x, p1.y, p2.x, p2.y);

		Renderer::SetDisplayColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	}

	void TextureManager::DrawRectWorldSpace(SDL_Rect rect, float angleDegrees, SDL_Color color, bool filled)
	{
		auto renderer = Renderer::GetRenderer();
		auto camera = Renderer::GetActiveCamera();

		if (renderer == nullptr || camera == nullptr || camera->GetRenderTarget() == nullptr)
			return;

		SDL_FPoint corners[4] =
		{
			{-rect.w / 2.0f, -rect.h / 2.0f},
			{ rect.w / 2.0f, -rect.h / 2.0f},
			{ rect.w / 2.0f,  rect.h / 2.0f},
			{-rect.w / 2.0f,  rect.h / 2.0f}
		};

		auto cx = rect.x + rect.w * 0.5f;
		auto cy = rect.y + rect.h * 0.5f;

		auto angleRad = angleDegrees * static_cast<float>(MathConstants::PI / 180.0f);
		auto cosA = cosf(angleRad);
		auto sinA = sinf(angleRad);

		for (auto& p : corners)
		{
			auto x = p.x * cosA - p.y * sinA;
			auto y = p.x * sinA + p.y * cosA;

			auto worldPos = Vector2F(x + cx, y + cy);
			auto screenPos = camera->WorldToScreen(worldPos);

			p.x = screenPos.x;
			p.y = screenPos.y;
		}

		// Culling via camera
		auto minX = corners[0].x, maxX = corners[0].x;
		auto minY = corners[0].y, maxY = corners[0].y;

		for (auto i = 1; i < 4; ++i)
		{
			minX = std::min(minX, corners[i].x);
			maxX = std::max(maxX, corners[i].x);
			minY = std::min(minY, corners[i].y);
			maxY = std::max(maxY, corners[i].y);
		}

		SDL_Rect screenBounds =
		{
			static_cast<int>(minX),
			static_cast<int>(minY),
			static_cast<int>(maxX - minX),
			static_cast<int>(maxY - minY)
		};

		if (!camera->IsVisible(screenBounds))
			return;

		if (filled)
		{
			SDL_Vertex vertices[6] =
			{
				{corners[0], color, {0,0}},
				{corners[1], color, {0,0}},
				{corners[2], color, {0,0}},
				{corners[2], color, {0,0}},
				{corners[3], color, {0,0}},
				{corners[0], color, {0,0}}
			};

			SDL_RenderGeometry(renderer, nullptr, vertices, 6, nullptr, 0);
		}
		else
		{
			auto oldColor = Renderer::GetDisplayColor();
			Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

			for (auto i = 0; i < 4; ++i)
			{
				SDL_RenderDrawLineF(
					renderer, 
					corners[i].x, corners[i].y,
					corners[(i + 1) % 4].x, corners[(i + 1) % 4].y);
			}

			Renderer::SetDisplayColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
		}
	}

	void TextureManager::DrawRectScreenSpace(SDL_Rect rect, float angleDegrees, SDL_Color color, bool filled)
	{
		auto renderer = Renderer::GetRenderer();

		if (renderer == nullptr)
			return;

		auto cx = rect.x + rect.w * 0.5f;
		auto cy = rect.y + rect.h * 0.5f;

		auto angleRad = angleDegrees * static_cast<float>(MathConstants::PI / 180.0f);
		auto cosA = cosf(angleRad);
		auto sinA = sinf(angleRad);

		SDL_FPoint corners[4] =
		{
			{-rect.w / 2.0f, -rect.h / 2.0f},
			{ rect.w / 2.0f, -rect.h / 2.0f},
			{ rect.w / 2.0f,  rect.h / 2.0f},
			{-rect.w / 2.0f,  rect.h / 2.0f}
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
			SDL_Vertex vertices[6] =
			{
				{corners[0], color, {0,0}},
				{corners[1], color, {0,0}},
				{corners[2], color, {0,0}},
				{corners[2], color, {0,0}},
				{corners[3], color, {0,0}},
				{corners[0], color, {0,0}}
			};

			SDL_RenderGeometry(renderer, nullptr, vertices, 6, nullptr, 0);
		}
		else
		{
			auto oldColor = Renderer::GetDisplayColor();
			Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

			for (int i = 0; i < 4; ++i)
			{
				SDL_RenderDrawLineF(
					renderer,
					corners[i].x, corners[i].y,
					corners[(i + 1) % 4].x, corners[(i + 1) % 4].y);
			}

			Renderer::SetDisplayColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
		}
	}

	void TextureManager::DrawCircleWorldSpace(Circle circle, SDL_Color color, bool filled)
	{
		auto renderer = Renderer::GetRenderer();
		auto camera = Renderer::GetActiveCamera();

		if (renderer == nullptr || camera == nullptr)
			return;

		auto screenCenter = circle.position;
		auto radius = circle.radius;

		screenCenter = camera->WorldToScreen(circle.position);
		radius *= camera->GetZoom();

		auto transformedCircle = Circle(screenCenter, radius);

		if (!camera->IsVisible(transformedCircle))
			return;

		DrawCircleScreenSpace(transformedCircle, color, filled);
	}

	void TextureManager::DrawCircleScreenSpace(Circle circle, SDL_Color color, bool filled)
	{
		auto renderer = Renderer::GetRenderer();

		if (renderer == nullptr)
			return;

		auto screenCenter = circle.position;
		auto radius = circle.radius;

		// Increase segments for smoother circles
		const auto segments = 64;

		if (filled)
		{
			std::vector<SDL_Vertex> vertices(segments + 2);
			std::vector<int> indices(segments * 3);

			vertices[0] = { {screenCenter.x, screenCenter.y}, color, {0.5f, 0.5f} };

			// Vertices
			for (auto i = 0; i <= segments; ++i)
			{
				auto angle = (2.0f * MathConstants::PI_f * i) / segments;
				vertices[i + 1] =
				{
					.position = { screenCenter.x + cosf(angle) * radius, screenCenter.y + sinf(angle) * radius },
					.color = color,
					.tex_coord = { (cosf(angle) + 1.0f) * 0.5f, (sinf(angle) + 1.0f) * 0.5f }
				};
			}

			// Indices (Each triangle connects the center to 2 outer points)
			for (auto i = 0; i < segments; ++i)
			{
				indices[i * 3 + 0] = 0;
				indices[i * 3 + 1] = i + 1;
				indices[i * 3 + 2] = i + 2;
			}

			SDL_RenderGeometry(renderer, nullptr, vertices.data(), (int)vertices.size(), indices.data(), (int)indices.size());
		}
		else
		{
			auto oldRenderColor = Renderer::GetDisplayColor();

			Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

			std::vector<SDL_Point> points(segments + 1);

			for (auto i = 0; i <= segments; ++i)
			{
				auto angle = (2.0f * MathConstants::PI_f * i) / segments;

				points[i].x = static_cast<int>(screenCenter.x + cosf(angle) * radius);
				points[i].y = static_cast<int>(screenCenter.y + sinf(angle) * radius);
			}

			SDL_RenderDrawLines(renderer, points.data(), (int)points.size());

			Renderer::SetDisplayColor(oldRenderColor.r, oldRenderColor.g, oldRenderColor.b, oldRenderColor.a);
		}
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
		auto renderer = Renderer::GetRenderer();
		auto camera = Renderer::GetActiveCamera();

		if (texture == nullptr || renderer == nullptr || camera == nullptr)
			return;

		auto texturePtr = texture.get();

		auto pos = Vector2F::Zero;
		auto size = Vector2F::Zero;

		if (!dstRect)
		{
			int w, h;
			SDL_QueryTexture(texture.get(), nullptr, nullptr, &w, &h);
			
			size = Vector2F(float(w), float(h));
		}
		else
		{
			pos = Vector2F(float(dstRect->x), float(dstRect->y));
			size = Vector2F(float(dstRect->w), float(dstRect->h));
		}

		pos = camera->WorldToScreen(pos);
		size *= camera->GetZoom();

		auto dest = SDL_Rect
		{
			.x = static_cast<int>(pos.x),
			.y = static_cast<int>(pos.y),
			.w = static_cast<int>(size.x),
			.h = static_cast<int>(size.y)
		};

		if (!camera->IsVisible(dest))
			return;

		// Backup texture state
		Uint8 oldAlpha, oldR, oldG, oldB;
		SDL_GetTextureAlphaMod(texturePtr, &oldAlpha);
		SDL_GetTextureColorMod(texturePtr, &oldR, &oldG, &oldB);

		SDL_SetTextureAlphaMod(texturePtr, alpha);
		SDL_SetTextureColorMod(texturePtr, colorMod.r, colorMod.g, colorMod.b);

		SDL_Point fallbackOrigin
		{
			static_cast<int>(std::roundf(dest.x * 0.5f)),
			static_cast<int>(std::roundf(dest.y * 0.5f))
		};

		SDL_RenderCopyEx(
			renderer,
			texturePtr,
			srcRect,
			&dest,
			angle,
			rotationOrigin ? rotationOrigin : &fallbackOrigin,
			flip);

		// Restore texture state
		SDL_SetTextureAlphaMod(texturePtr, oldAlpha);
		SDL_SetTextureColorMod(texturePtr, oldR, oldG, oldB);
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
		auto renderer = Renderer::GetRenderer();

		if (texture == nullptr || renderer == nullptr)
			return;

		auto dest = dstRect ? *dstRect : SDL_Rect{ 0,0,0,0 };

		if (!dstRect)
		{
			int w, h;
			SDL_QueryTexture(texture.get(), nullptr, nullptr, &w, &h);
			
			dest = SDL_Rect
			{ 
				.x = 0, 
				.y = 0, 
				.w = w, 
				.h = h 
			};
		}

		auto texturePtr = texture.get();

		// Backup texture state
		Uint8 oldAlpha, oldR, oldG, oldB;
		SDL_GetTextureAlphaMod(texturePtr, &oldAlpha);
		SDL_GetTextureColorMod(texturePtr, &oldR, &oldG, &oldB);

		SDL_SetTextureAlphaMod(texturePtr, alpha);
		SDL_SetTextureColorMod(texturePtr, colorMod.r, colorMod.g, colorMod.b);

		SDL_Point fallbackOrigin
		{
			static_cast<int>(std::roundf(dest.x * 0.5f)),
			static_cast<int>(std::roundf(dest.y * 0.5f))
		};

		SDL_RenderCopyEx(
			renderer,
			texturePtr,
			srcRect,
			&dest,
			angle,
			rotationOrigin ? rotationOrigin : &fallbackOrigin,
			flip);

		// Restore texture state
		SDL_SetTextureAlphaMod(texturePtr, oldAlpha);
		SDL_SetTextureColorMod(texturePtr, oldR, oldG, oldB);
	}

	void TextureManager::DrawPixelWorldSpace(const Vector2F& worldPos, SDL_Color color)
	{
		auto camera = Renderer::GetActiveCamera();
		auto renderer = Renderer::GetRenderer();

		if (camera == nullptr || renderer == nullptr)
			return;

		// Convert world position to screen using camera
		auto worldToScreenPos = camera->WorldToScreen(worldPos);

		auto testRect = SDL_Rect
		{ 
			.x = static_cast<int>(worldToScreenPos.x),
			.y = static_cast<int>(worldToScreenPos.y),
			.w = 1, 
			.h = 1 
		};

		if (!camera->IsVisible(testRect))
			return;

		auto oldColor = Renderer::GetDisplayColor();
		Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

		SDL_RenderDrawPoint(
			renderer,
			static_cast<int>(worldToScreenPos.x),
			static_cast<int>(worldToScreenPos.y));

		Renderer::SetDisplayColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	}

	void TextureManager::DrawPixelScreenSpace(const Vector2F& screenPos, SDL_Color color)
	{
		auto renderer = Renderer::GetRenderer();

		if (renderer == nullptr)
			return;

		auto oldColor = Renderer::GetDisplayColor();

		Renderer::SetDisplayColor(color.r, color.g, color.b, color.a);

		SDL_RenderDrawPoint(
			renderer,
			static_cast<int>(screenPos.x),
			static_cast<int>(screenPos.y));

		Renderer::SetDisplayColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	}
}