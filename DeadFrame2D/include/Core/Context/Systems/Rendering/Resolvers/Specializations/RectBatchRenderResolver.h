#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Rendering/Pipeline/Shapes/RectBatchRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	template<>
	struct RenderResolver<DeadFrame2D::Data::RectBatchRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::RectBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			for (const auto& rect : renderData.rectBatch)
			{
				SDL_FRect destRect = rect.destRect;

				if (camera != nullptr && requiresScreenSpaceConversion)
				{
					auto screenPos = camera->WorldToScreen(Vector2F(destRect.x, destRect.y));

					destRect.x = screenPos.x;
					destRect.y = screenPos.y;
					destRect.w *= camera->GetZoom();
					destRect.h *= camera->GetZoom();
				}

				renderBackend.DrawRect(
					destRect,
					rect.rotation,
					rect.color,
					rect.filled);
			}
		}

		static std::optional<DeadFrame2D::Data::RectBatchRenderData> Cull(
			const DeadFrame2D::Data::RectBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera)
		{
			using namespace DeadFrame2D::Data;
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			auto filtered = RectBatchRenderData();
			filtered.rectBatch.reserve(renderData.rectBatch.size());

			for (const auto& rect : renderData.rectBatch)
			{
				SDL_FRect screenRect = rect.destRect;

				auto screenPos = camera->WorldToScreen(Vector2F(screenRect.x, screenRect.y));

				screenRect.x = screenPos.x;
				screenRect.y = screenPos.y;
				screenRect.w *= camera->GetZoom();
				screenRect.h *= camera->GetZoom();

				if (Collision::RectVsRect(screenRect, camera->GetViewBox()))
				{
					filtered.rectBatch.push_back(rect);
				}
			}

			return filtered.rectBatch.empty() ? std::nullopt : std::optional{ std::move(filtered) };
		}
	};
}