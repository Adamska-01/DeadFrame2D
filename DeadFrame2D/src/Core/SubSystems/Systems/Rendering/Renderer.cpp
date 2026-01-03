#include "Constants/MathConstants.h"
#include "Core/Debugging/Debug.h"
#include "Core/Math/Circle.h"
#include "Core/SubSystems/Systems/Rendering/Renderer.h"
#include "Core/SubSystems/Systems/Window.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Components/UI/Canvas.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Renderer/RenderTargetSizeChangedEvent.h"
#include <SDL.h>


namespace DeadFrame2D::Core
{
	using namespace Shared::Models;

	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Data;


	SDL_Renderer* Renderer::renderer = nullptr;


	Renderer::Renderer(SDL_Window* window, const RendererConfig& config)
	{
	#if _DEBUG
		DBG_ASSERT_MSG(window, "Window initialisation failed: %s\n", SDL_GetError());
	#endif

		//startup
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "Failed to Initialize SDL_VIDEO: " << SDL_GetError() << std::endl;
		}

		//create the renderer
		renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED);

	#if _DEBUG
		DBG_ASSERT_MSG(renderer, "Renderer initialisation failed: %s\n", SDL_GetError());
	#endif

		SetResolutionTarget({ config.width, config.height });

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		std::cout << "[Info] SDL_VIDEO successfully initialized." << std::endl;
	}

	Renderer::~Renderer()
	{
		if (renderer == nullptr) 
		{
			std::cout << "[Info] Renderer is already null, no need to destroy." << std::endl;

			return;
		}

		SDL_DestroyRenderer(renderer);

		SDL_Quit();

		std::cout << "[Info] Renderer successfully destroyed and SDL_VIDEO successfully quit." << std::endl;
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
		// TODO: Make this a constant somewhere
		const auto segments = 64;

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

	void Renderer::DrawFromTask(RenderTask& renderTask, ComponentHandle<Camera> camera, bool requiresSreenSpaceConversion)
	{
		switch (renderTask.renderType)
		{
			case RenderPrimitive::SPRITE:
				{
					auto& renderData = std::get<SpriteRenderData>(renderTask.renderData);

					if (camera != nullptr && requiresSreenSpaceConversion)
					{
						auto screenPos = camera->WorldToScreen(Vector2F(renderData.destRect.x, renderData.destRect.y));

						renderData.destRect.x = screenPos.x;
						renderData.destRect.y = screenPos.y;
						renderData.destRect.w *= camera->GetZoom();
						renderData.destRect.h *= camera->GetZoom();

						if (!camera->IsVisible(renderData.destRect))
							return;
					}

					DrawTexture(
						renderData.texture,
						&renderData.srcRect,
						&renderData.destRect,
						&renderData.rotationOrigin,
						renderData.rotation,
						renderData.flip,
						renderData.colorMod);
				}
				break;

			case RenderPrimitive::RECT:
				{
					auto& renderData = std::get<RectRenderData>(renderTask.renderData);

					if (camera != nullptr && requiresSreenSpaceConversion)
					{
						auto screenPos = camera->WorldToScreen(Vector2F(renderData.destRect.x, renderData.destRect.y));

						renderData.destRect.x = screenPos.x;
						renderData.destRect.y = screenPos.y;
						renderData.destRect.w *= camera->GetZoom();
						renderData.destRect.h *= camera->GetZoom();

						if (!camera->IsVisible(renderData.destRect))
							return;
					}

					DrawRect(
						renderData.destRect,
						renderData.rotation,
						renderData.color,
						renderData.filled);
				}
				break;

			case RenderPrimitive::CIRCLE:
				{
					auto& renderData = std::get<CircleRenderData>(renderTask.renderData);

					if (camera != nullptr && requiresSreenSpaceConversion)
					{
						renderData.center = camera->WorldToScreen(renderData.center);
						renderData.radius *= camera->GetZoom();

						if (!camera->IsVisible(Circle(renderData.center, renderData.radius)))
							return;
					}

					DrawCircle(
						renderData.center,
						renderData.radius,
						renderData.color,
						renderData.filled);
				}
				break;

			case RenderPrimitive::LINE:
				{
					auto& renderData = std::get<LineRenderData>(renderTask.renderData);

					if (camera != nullptr && requiresSreenSpaceConversion)
					{
						renderData.p1 = camera->WorldToScreen(renderData.p1);
						renderData.p2 = camera->WorldToScreen(renderData.p2);

						if (!camera->IsVisible(renderData.p1, renderData.p2))
							return;
					}

					DrawLine(
						renderData.p1,
						renderData.p2,
						renderData.color);
				}
				break;
			
			case RenderPrimitive::POINT:
				{
					auto& renderData = std::get<PointRenderData>(renderTask.renderData);

					if (camera != nullptr && requiresSreenSpaceConversion)
					{
						renderData.pos = camera->WorldToScreen(renderData.pos);

						if (!camera->IsVisible(renderData.pos, renderData.pos))
							return;
					}

					DrawPixel(renderData.pos, renderData.color);
				}
				break;
		}
	}

	void Renderer::ClearAndPresentBuffer()
	{
		std::sort(
			renderTasks.begin(), 
			renderTasks.end(),
			[](const RenderTask& a, const RenderTask& b)
			{
				return a.GetSortKey() < b.GetSortKey();
			});

		// ==============================================================
		// 1) CAMERA PASSES: WORLD + DEBUG_WORLD + SCREEN_SPACE_CAMERA_UI
		// ==============================================================
		for (auto camera : Camera::GetCameras())
		{
			if (!camera->IsActive())
				continue;

			auto cameraHandle = camera->GetHandleAs<Camera>();

			SDL_SetRenderTarget(renderer, camera->GetRenderTarget());

			for (auto& task : renderTasks)
			{
				// All the WORLD ones are guaranteed come before SCREEN_SPACE_CAMERA_UI thanks to the sorting, so it's fine to do it in a single loop.
				switch (task.renderPhase)
				{
					case RenderPhase::WORLD:
					case RenderPhase::DEBUG_WORLD:
						DrawFromTask(task, cameraHandle);
						break;

					case RenderPhase::SCREEN_SPACE_CAMERA_UI:
						if (task.canvas->GetRenderCamera() == cameraHandle)
							DrawFromTask(task, cameraHandle, false);
						break;

					default:
						break;
				}
			}
		}

		// ==============================================================
		// 2) COMPOSITE CAMERA RENDER TARGETS -> BACKBUFFER
		// ==============================================================
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderClear(renderer);

		for (auto camera : Camera::GetCameras())
		{
			if (!camera->IsActive())
				continue;

			auto cameraHandle = camera->GetHandleAs<Camera>();
			auto viewport = cameraHandle->GetNormalizedViewBox();

			SDL_RenderCopy(
				renderer,
				cameraHandle->GetRenderTarget(),
				nullptr,
				&viewport);
		}

		// =========================================================
		// 3) SCREEN-SPACE OVERLAY UI
		// 4) DEBUG SCREEN OVERLAY
		// =========================================================
		for (auto& task : renderTasks)
		{
			switch (task.renderPhase)
			{
				case RenderPhase::SCREEN_SPACE_OVERLAY_UI:
				case RenderPhase::DEBUG_OVERLAY:
					DrawFromTask(task, {}, false);
					break;

				default:
					break;
			}
		}

		SDL_RenderPresent(renderer);
	}

	SDL_Renderer* Renderer::GetRenderer()
	{
		return renderer;
	}

	SDL_Color Renderer::GetDisplayColor()
	{
		Uint8 r, g, b, a;

		SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

		return SDL_Color(r, g, b, a);
	}

	Vector2I Renderer::GetResolutionTarget()
	{
		auto width = 0;
		auto height = 0;

		if (renderer != nullptr)
		{
			SDL_RenderGetLogicalSize(renderer, &width, &height);
		}

		return Vector2I(width, height);
	}

	void Renderer::SetViewport(const SDL_Rect& viewPort)
	{
		SDL_RenderSetViewport(renderer, &viewPort);
	}

	void Renderer::SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
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
		DBG_ASSERT_MSG(!result, "Background color initialisation failed: %s\n", SDL_GetError());
	#endif
	}

	void Renderer::SetResolutionTarget(Vector2I targetResolution)
	{
		auto width = targetResolution.x;
		auto height = targetResolution.y;

		if (width <= 0 || height <= 0)
			return;

		SDL_RenderSetLogicalSize(renderer, width, height);

		EventDispatcher::SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I(width, height)));
	}
}