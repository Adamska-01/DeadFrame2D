#if DEBUG
#include <cassert>
#endif
#include "Constants/MathConstants.h"
#include "Constants/Rendering/DrawConstants.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/Rendering/RenderPipeline.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Renderer/RenderTargetSizeChangedEvent.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Models;
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Data;


	Renderer* Renderer::instance = nullptr;


	Renderer::Renderer(SDL_Window* window, const RendererConfig& config)
	{
		assert(instance == nullptr && "Renderer was already initialized!");

		instance = this;

#if DEBUG
		assert(window && SDL_GetError());
#endif

		//startup
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "Failed to Initialize SDL_VIDEO: " << SDL_GetError() << std::endl;
		}

		renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED);

#if DEBUG
		assert(renderer && SDL_GetError());
#endif

		SetResolutionTarget({ config.width, config.height });

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		std::cout << "[Info] SDL_VIDEO successfully initialized." << std::endl;

		renderPipeline = std::make_unique<RenderPipeline>();
	}

	Renderer::~Renderer()
	{
		if (renderer == nullptr) 
		{
			std::cout << "[Info] Renderer is already null, no need to destroy." << std::endl;

			return;
		}

		SDL_DestroyRenderer(renderer);

		std::cout << "[Info] Renderer successfully destroyed." << std::endl;

		renderPipeline.reset();

		instance = nullptr;
	}

	void Renderer::BeginFrame()
	{

	}

	void Renderer::PreUpdate(float deltaTime)
	{

	}

	void Renderer::EndUpdate(float deltaTime)
	{

	}

	void Renderer::EndDraw()
	{

	}

	void Renderer::DrawPixel(const Vector2F& pixelPos, SDL_Color color)
	{
		if (renderer == nullptr)
			return;

		auto previousBackgroundColor = GetDisplayColor();

		SetDisplayColor(color.r, color.g, color.b, color.a);

		SDL_RenderDrawPointF(renderer, pixelPos.x, pixelPos.y);

		SetDisplayColor(previousBackgroundColor.r, previousBackgroundColor.g, previousBackgroundColor.b, previousBackgroundColor.a);
	}

	void Renderer::DrawLine(const Vector2F& p1, const Vector2F& p2, SDL_Color color)
	{
		if (renderer == nullptr)
			return;

		auto previousBackgroundColor = Renderer::GetDisplayColor();

		SetDisplayColor(color.r, color.g, color.b, color.a);

		SDL_RenderDrawLineF(renderer, p1.x, p1.y, p2.x, p2.y);

		SetDisplayColor(previousBackgroundColor.r, previousBackgroundColor.g, previousBackgroundColor.b, previousBackgroundColor.a);
	}

	void Renderer::DrawRect(SDL_FRect rect, float angleDegrees, SDL_Color color, bool filled)
	{
		if (renderer == nullptr)
			return;

		const auto cx = rect.x + rect.w * 0.5f;
		const auto cy = rect.y + rect.h * 0.5f;

		const auto angleRad = angleDegrees * (MathConstants::PI_f / 180.0f);
		const auto cosA = cosf(angleRad);
		const auto sinA = sinf(angleRad);

		// Local corners relative to center
		SDL_FPoint corners[4] =
		{
			{-rect.w / 2.0f, -rect.h / 2.0f},
			{ rect.w / 2.0f, -rect.h / 2.0f},
			{ rect.w / 2.0f,  rect.h / 2.0f},
			{-rect.w / 2.0f,  rect.h / 2.0f}
		};

		// Rotate and translate corners
		for (auto& p : corners)
		{
			auto x = p.x * cosA - p.y * sinA + cx;
			auto y = p.x * sinA + p.y * cosA + cy;
			
			p.x = x;
			p.y = y;
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
			const SDL_Color oldColor = GetDisplayColor();

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

	void Renderer::DrawCircle(const Vector2F& center, float radius, SDL_Color color, bool filled)
	{
		if (renderer == nullptr)
			return;

		// Increase segments for smoother circles
		const auto segments = DrawConstants::CIRCLE_SEGMENTS;

		if (filled)
		{
			std::vector<SDL_Vertex> vertices(segments + 2);
			std::vector<int> indices(segments * 3);

			vertices[0] = { {center.x, center.y}, color, {0.5f, 0.5f} };

			// Vertices
			for (auto i = 0; i <= segments; ++i)
			{
				auto angle = (2.0f * MathConstants::PI_f * i) / segments;
				vertices[i + 1] =
				{
					.position = { center.x + cosf(angle) * radius, center.y + sinf(angle) * radius },
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

	void Renderer::DrawTexture(SDL_Texture* texture, const SDL_Rect* srcRect, const SDL_FRect* dstRect, const SDL_FPoint* rotationOrigin, float angle, SDL_RendererFlip flip, SDL_Color colorMod)
	{
		if (texture == nullptr || renderer == nullptr)
			return;

		// Determine destination rectangle
		SDL_FRect dest{};
		if (dstRect)
		{
			dest = *dstRect;
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

		SDL_SetTextureAlphaMod(texture, colorMod.a);
		SDL_SetTextureColorMod(texture, colorMod.r, colorMod.g, colorMod.b);

		// Determine rotation origin
		SDL_FPoint fallbackOrigin
		{
			dest.w * 0.5f,
			dest.h * 0.5f
		};

		SDL_RenderCopyExF(
			renderer,
			texture,
			srcRect,
			&dest,
			angle,
			rotationOrigin ? rotationOrigin : &fallbackOrigin,
			flip);

		// Restore texture state
		SDL_SetTextureAlphaMod(texture, oldAlpha);
		SDL_SetTextureColorMod(texture, oldR, oldG, oldB);
	}

	void Renderer::SetRenderTarget(SDL_Texture* renderTarget)
	{
		SDL_SetRenderTarget(renderer, renderTarget);
	}

	void Renderer::ClearCurrentRenderTarget()
	{
		SDL_RenderClear(renderer);
	}

	void Renderer::ClearAndPresentBuffer()
	{
		instance->renderPipeline->Execute(*instance, renderTasks);

		SDL_RenderPresent(instance->renderer);

		for (auto& phaseMap : renderTasks)
		{
			phaseMap.clear();
		}
	}

	SDL_Renderer* Renderer::GetRenderer()
	{
		return instance->renderer;
	}

	SDL_Color Renderer::GetDisplayColor()
	{
		auto renderer = instance->renderer;

		Uint8 r, g, b, a;

		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

		return SDL_Color(r, g, b, a);
	}

	Vector2I Renderer::GetResolutionTarget()
	{
		auto width = 0;
		auto height = 0;

		auto renderer = instance->renderer;

		if (renderer != nullptr)
		{
			SDL_RenderGetLogicalSize(renderer, &width, &height);
		}

		return Vector2I(width, height);
	}

	void Renderer::SetViewport(const SDL_Rect& viewPort)
	{
		SDL_RenderSetViewport(instance->renderer, &viewPort);
	}

	void Renderer::SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		auto renderer = instance->renderer;

		if (renderer == nullptr)
			return;

		//set background color
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

	void Renderer::SetResolutionTarget(Vector2I targetResolution)
	{
		auto width = targetResolution.x;
		auto height = targetResolution.y;

		if (width <= 0 || height <= 0)
			return;

		SDL_RenderSetLogicalSize(instance->renderer, width, height);

		EventDispatcher::SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I(width, height)));
	}
}