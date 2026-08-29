#if DEBUG
#include <cassert>
#endif
#include "Constants/MathConstants.h"
#include "Constants/Rendering/DrawConstants.h"
#include "Core/Math/MathUtils.h"
#include "Converters/Rendering/RenderingConversions.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
#include "Factories/Products/Context/Systems/Rendering/SDLRenderBackend.h"
#include <SDL.h>
#include <iostream>


namespace DF2D::Internal
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Internal::RenderingConversions;


	SDLRenderBackend::SDLRenderBackend(SDL_Window* window, const Models::RendererConfig& config, EventDispatcher& eventDispatcher)
		: eventDispatcher(eventDispatcher)
	{
		renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED);

#if DEBUG
		assert(renderer && SDL_GetError());
#endif

		SetResolutionTarget({ config.width, config.height });

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		std::cout << "[Info] SDL render backend successfully initialized." << std::endl;
	}

	SDLRenderBackend::~SDLRenderBackend()
	{
		for (auto& [id, tex] : textureRegistry.textures)
		{
			if (tex == nullptr)
				continue;

			SDL_DestroyTexture(tex);
		}

		textureRegistry.textures.clear();

		if (renderer == nullptr)
		{
			std::cout << "[Info] Render backend renderer is already null, no need to destroy." << std::endl;
		}
		else
		{
			SDL_DestroyRenderer(renderer);

			std::cout << "[Info] SDL render backend successfully destroyed." << std::endl;
		}
	}


	void SDLRenderBackend::ApplyDrawBlendMode(Data::BlendMode blendMode)
	{
		if (blendMode == currentDrawBlendMode)
			return;

		SDL_SetRenderDrawBlendMode(renderer, ToSDLBlendMode(blendMode));

		currentDrawBlendMode = blendMode;
	}

	void SDLRenderBackend::DrawPixel(const Vector2F& pixelPos, Color color, Data::BlendMode blendMode)
	{
		if (renderer == nullptr)
			return;

		ApplyDrawBlendMode(blendMode);

		auto previousBackgroundColor = GetDisplayColor();

		SetDisplayColor(color.r, color.g, color.b, color.a);

		SDL_RenderDrawPointF(renderer, pixelPos.x, pixelPos.y);

		SetDisplayColor(previousBackgroundColor.r, previousBackgroundColor.g, previousBackgroundColor.b, previousBackgroundColor.a);
	}

	void SDLRenderBackend::DrawLine(const Vector2F& p1, const Vector2F& p2, Color color, Data::BlendMode blendMode)
	{
		if (renderer == nullptr)
			return;

		ApplyDrawBlendMode(blendMode);

		auto previousBackgroundColor = GetDisplayColor();

		SetDisplayColor(color.r, color.g, color.b, color.a);

		SDL_RenderDrawLineF(renderer, p1.x, p1.y, p2.x, p2.y);

		SetDisplayColor(previousBackgroundColor.r, previousBackgroundColor.g, previousBackgroundColor.b, previousBackgroundColor.a);
	}

	void SDLRenderBackend::DrawRect(RectF rect, float angleDegrees, Color color, bool filled, Data::BlendMode blendMode)
	{
		if (renderer == nullptr)
			return;

		ApplyDrawBlendMode(blendMode);

		const auto cx = rect.x + rect.w * 0.5f;
		const auto cy = rect.y + rect.h * 0.5f;

		const auto angleRad = Core::MathUtils::ToRadians(angleDegrees);
		const auto cosA = cosf(angleRad);
		const auto sinA = sinf(angleRad);

		SDL_FPoint corners[4] =
		{
			{-rect.w / 2.0f, -rect.h / 2.0f},
			{ rect.w / 2.0f, -rect.h / 2.0f},
			{ rect.w / 2.0f,  rect.h / 2.0f},
			{-rect.w / 2.0f,  rect.h / 2.0f}
		};

		for (auto& p : corners)
		{
			auto x = p.x * cosA - p.y * sinA + cx;
			auto y = p.x * sinA + p.y * cosA + cy;

			p.x = x;
			p.y = y;
		}

		auto sdlColor = ToSDLColor(color);

		if (filled)
		{
			SDL_Vertex vertices[6] =
			{
				{corners[0], sdlColor, {0,0}},
				{corners[1], sdlColor, {0,0}},
				{corners[2], sdlColor, {0,0}},
				{corners[2], sdlColor, {0,0}},
				{corners[3], sdlColor, {0,0}},
				{corners[0], sdlColor, {0,0}}
			};

			SDL_RenderGeometry(renderer, nullptr, vertices, 6, nullptr, 0);
		}
		else
		{
			const auto oldColor = GetDisplayColor();

			SetDisplayColor(color.r, color.g, color.b, color.a);

			for (auto i = 0; i < 4; ++i)
			{
				SDL_RenderDrawLineF(
					renderer,
					corners[i].x,
					corners[i].y,
					corners[(i + 1) % 4].x,
					corners[(i + 1) % 4].y);
			}

			SetDisplayColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
		}
	}

	void SDLRenderBackend::DrawCircle(const Vector2F& center, float radius, Color color, bool filled, Data::BlendMode blendMode)
	{
		if (renderer == nullptr)
			return;

		ApplyDrawBlendMode(blendMode);

		const auto segments = DrawConstants::CIRCLE_SEGMENTS;

		auto sdlColor = ToSDLColor(color);

		if (filled)
		{
			std::vector<SDL_Vertex> vertices(segments + 2);
			std::vector<int> indices(segments * 3);

			vertices[0] = { {center.x, center.y}, sdlColor, {0.5f, 0.5f} };

			for (auto i = 0; i <= segments; ++i)
			{
				auto angle = (2.0f * MathConstants::PI_f * i) / segments;
				vertices[i + 1] =
				{
					.position = { center.x + cosf(angle) * radius, center.y + sinf(angle) * radius },
					.color = sdlColor,
					.tex_coord = { (cosf(angle) + 1.0f) * 0.5f, (sinf(angle) + 1.0f) * 0.5f }
				};
			}

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
			auto oldRenderColor = GetDisplayColor();

			SetDisplayColor(color.r, color.g, color.b, color.a);

			std::vector<SDL_FPoint> points(segments + 1);

			for (auto i = 0; i <= segments; ++i)
			{
				auto angle = (2.0f * MathConstants::PI_f * i) / segments;

				points[i].x = center.x + cosf(angle) * radius;
				points[i].y = center.y + sinf(angle) * radius;
			}

			SDL_RenderDrawLinesF(renderer, points.data(), segments + 1);

			SetDisplayColor(oldRenderColor.r, oldRenderColor.g, oldRenderColor.b, oldRenderColor.a);
		}
	}

	void SDLRenderBackend::DrawTexture(
		Data::TextureID textureID,
		const std::optional<RectI>& srcRect,
		const std::optional<RectF>& dstRect,
		const std::optional<Vector2F>& rotationOrigin,
		float angle,
		Data::RenderFlip flip,
		Color colorMod,
		Data::BlendMode blendMode)
	{
		auto* texture = textureRegistry.GetTexture(textureID);

		if (texture == nullptr || renderer == nullptr)
			return;

		SDL_FRect dest{};
		if (dstRect.has_value())
		{
			dest = ToSDLFRect(*dstRect);
		}
		else
		{
			int w, h;
			SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

			dest = SDL_FRect{ 0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h) };
		}

		Uint8 oldAlpha, oldR, oldG, oldB;
		SDL_GetTextureAlphaMod(texture, &oldAlpha);
		SDL_GetTextureColorMod(texture, &oldR, &oldG, &oldB);

		auto sdlColorMod = ToSDLColor(colorMod);
		SDL_SetTextureAlphaMod(texture, sdlColorMod.a);
		SDL_SetTextureColorMod(texture, sdlColorMod.r, sdlColorMod.g, sdlColorMod.b);

		SDL_SetTextureBlendMode(texture, ToSDLBlendMode(blendMode));

		SDL_FPoint fallbackOrigin
		{
			dest.w * 0.5f,
			dest.h * 0.5f
		};

		SDL_FPoint* originPtr = nullptr;
		SDL_FPoint sdlOrigin;
		if (rotationOrigin.has_value())
		{
			sdlOrigin = { rotationOrigin->x, rotationOrigin->y };
			originPtr = &sdlOrigin;
		}

		SDL_Rect sdlSrcRect;
		SDL_Rect* srcPtr = nullptr;
		if (srcRect.has_value())
		{
			sdlSrcRect = ToSDLRect(*srcRect);
			srcPtr = &sdlSrcRect;
		}

		SDL_RenderCopyExF(
			renderer,
			texture,
			srcPtr,
			&dest,
			angle,
			originPtr ? originPtr : &fallbackOrigin,
			ToSDLRenderFlip(flip));

		SDL_SetTextureAlphaMod(texture, oldAlpha);
		SDL_SetTextureColorMod(texture, oldR, oldG, oldB);
	}

	void SDLRenderBackend::SetRenderTarget(Data::TextureID renderTarget)
	{
		if (renderTarget <= 0)
		{
			SDL_SetRenderTarget(renderer, nullptr);
		}
		else
		{
			auto* tex = textureRegistry.GetTexture(renderTarget);

			if (tex != nullptr)
			{
				SDL_SetRenderTarget(renderer, tex);
			}
		}
	}

	void SDLRenderBackend::ClearCurrentRenderTarget()
	{
		ApplyDrawBlendMode(Data::BlendMode::NONE);

		SDL_RenderClear(renderer);
	}

	void SDLRenderBackend::Present()
	{
		SDL_RenderPresent(renderer);
	}

	Data::TextureID SDLRenderBackend::CreateRenderTarget(int w, int h)
	{
		auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

		if (!texture)
		{
			std::cerr << "Failed to create render target: " << SDL_GetError() << std::endl;

			return 0;
		}

		return textureRegistry.AddTexture(texture);
	}

	void SDLRenderBackend::DestroyTexture(Data::TextureID id)
	{
		textureRegistry.RemoveTexture(id, true);
	}

	Color SDLRenderBackend::GetDisplayColor()
	{
		Uint8 r, g, b, a;

		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

		return Color(r, g, b, a);
	}

	Vector2I SDLRenderBackend::GetResolutionTarget()
	{
		auto width = 0;
		auto height = 0;

		if (renderer != nullptr)
		{
			SDL_RenderGetLogicalSize(renderer, &width, &height);
		}

		return Vector2I(width, height);
	}

	void SDLRenderBackend::SetViewport(RectI viewPort)
	{
		auto sdlRect = ToSDLRect(viewPort);

		SDL_RenderSetViewport(renderer, &sdlRect);
	}

	void SDLRenderBackend::SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		if (renderer == nullptr)
			return;

		auto result = SDL_SetRenderDrawColor(
			renderer,
			r,
			g,
			b,
			a);

#if _DEBUG
		assert((result == 0) && SDL_GetError());
#endif
	}

	void SDLRenderBackend::SetResolutionTarget(Vector2I targetResolution)
	{
		auto width = targetResolution.x;
		auto height = targetResolution.y;

		if (width <= 0 || height <= 0)
			return;

		SDL_RenderSetLogicalSize(renderer, width, height);

		resolutionTarget = targetResolution;

		eventDispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I(width, height)));
	}
}