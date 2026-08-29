#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/SpriteRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::SpriteRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::SpriteRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			RectF destRect;

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
				renderData.srcRect,
				renderData.destRect.has_value() ? std::optional<RectF>(destRect) : std::nullopt,
				renderData.rotationOrigin,
				renderData.rotation,
				renderData.flip,
				renderData.colorMod,
				renderData.blendMode);
		}

		static std::optional<Data::SpriteRenderData> Cull(
			const Data::SpriteRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			using namespace DF2D::Utilities;


			if (!renderData.destRect.has_value() || camera == nullptr)
				return renderData;

			auto screenPos = camera->WorldToScreen(Vector2F(renderData.destRect->x, renderData.destRect->y));

			auto screenRect = RectF
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