#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/SpriteBatchRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	struct SpriteBatchRenderResolver
	{
		// Render
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::SpriteBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
		{
			for (const auto& sprite : renderData.spriteBatch)
			{
				SDL_FRect destRect;

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
					sprite.srcRect.has_value() ? &*sprite.srcRect : nullptr,
					sprite.destRect.has_value() ? &destRect : nullptr,
					sprite.rotationOrigin.has_value() ? &*sprite.rotationOrigin : nullptr,
					sprite.rotation,
					sprite.flip,
					sprite.colorMod);
			}
		}

		// Visibility Check
		std::optional<DeadFrame2D::Data::SpriteBatchRenderData> operator()(
			const DeadFrame2D::Data::SpriteBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera) const
		{
			using namespace DeadFrame2D::Data;
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			auto filtered = SpriteBatchRenderData();
			filtered.spriteBatch.reserve(renderData.spriteBatch.size()); // Trade memory cost for CPU stability

			for (const auto& sprite : renderData.spriteBatch)
			{
				if (!sprite.destRect.has_value())
				{
					filtered.spriteBatch.push_back(sprite);
				}

				auto screenPos = camera->WorldToScreen(Vector2F(sprite.destRect->x, sprite.destRect->y));

				auto screenRect = SDL_FRect
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