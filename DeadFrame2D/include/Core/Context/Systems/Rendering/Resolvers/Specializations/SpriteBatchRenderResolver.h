#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/SpriteBatchRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::SpriteBatchRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::SpriteBatchRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			for (const auto& sprite : renderData.spriteBatch)
			{
				RectF destRect;

				if (sprite.destRect.has_value())
				{
					destRect = *sprite.destRect;

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
					sprite.texture,
					sprite.srcRect,
					sprite.destRect.has_value() ? std::optional<RectF>(destRect) : std::nullopt,
					sprite.rotationOrigin,
					sprite.rotation,
					sprite.flip,
					sprite.colorMod);
			}
		}

		static std::optional<Data::SpriteBatchRenderData> Cull(
			const Data::SpriteBatchRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			using namespace DF2D::Data;
			using namespace DF2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			// Trade memory cost for CPU stability
			auto filtered = SpriteBatchRenderData();
			filtered.spriteBatch.reserve(renderData.spriteBatch.size());

			for (const auto& sprite : renderData.spriteBatch)
			{
				if (!sprite.destRect.has_value())
				{
					filtered.spriteBatch.push_back(sprite);
				}

				auto screenPos = camera->WorldToScreen(Vector2F(sprite.destRect->x, sprite.destRect->y));

				auto screenRect = RectF
				{
					.x = screenPos.x,
					.y = screenPos.y,
					.w = sprite.destRect->w * camera->GetZoom(),
					.h = sprite.destRect->h * camera->GetZoom()
				};

				if (Collision::RectVsRect(screenRect, camera->GetViewBox()))
				{
					filtered.spriteBatch.push_back(sprite);
				}
			}

			return filtered.spriteBatch.empty() ? std::nullopt : std::optional{ std::move(filtered) };
		}
	};
}