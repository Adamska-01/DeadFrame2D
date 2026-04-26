#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Rendering/Pipeline/Shapes/SpriteRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	template<>
	struct RenderResolver<DeadFrame2D::Data::SpriteRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::SpriteRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			SDL_FRect destRect;

			if (renderData.destRect.has_value())
			{ 
				destRect = *renderData.destRect;

				if (camera != nullptr && requiresScreenSpaceConversion)
				{
					auto screenPos = camera->WorldToScreen(Vector2F(destRect.x, destRect.y));

					destRect.x = screenPos.x;
					destRect.y = screenPos.y;
					destRect.w *= camera->GetZoom();
					destRect.h *= camera->GetZoom();
				}
			}

			renderBackend.DrawTexture(
				renderData.texture,
				renderData.srcRect.has_value() ? &*renderData.srcRect : nullptr,
				renderData.destRect.has_value() ? &destRect : nullptr,
				renderData.rotationOrigin.has_value() ? &*renderData.rotationOrigin : nullptr,
				renderData.rotation,
				renderData.flip,
				renderData.colorMod);
		}

		static std::optional<DeadFrame2D::Data::SpriteRenderData> Cull(
			const DeadFrame2D::Data::SpriteRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera)
		{
			using namespace DeadFrame2D::Utilities;


			// Always visible if no rect or no camera
			if (!renderData.destRect.has_value() || camera == nullptr)
				return renderData;

			auto screenPos = camera->WorldToScreen(Vector2F(renderData.destRect->x, renderData.destRect->y));

			auto screenRect = SDL_FRect
			{
				.x = screenPos.x,
				.y = screenPos.y,
				.w = renderData.destRect->w * camera->GetZoom(),
				.h = renderData.destRect->h * camera->GetZoom()
			};

			if (Collision::RectVsRect(screenRect, camera->GetViewBox()))
				return renderData;

			return std::nullopt;
		}
	};
}