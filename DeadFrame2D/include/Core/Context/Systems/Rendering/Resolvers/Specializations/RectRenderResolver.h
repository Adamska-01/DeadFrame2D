#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/RectRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::RectRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::RectRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			RectF destRect = renderData.destRect;

			if (camera != nullptr && requiresScreenSpaceConversion)
			{
				auto screenPos = camera->WorldToScreen(Vector2F(renderData.destRect.x, renderData.destRect.y));

				destRect.x = screenPos.x;
				destRect.y = screenPos.y;
				destRect.w *= camera->GetZoom();
				destRect.h *= camera->GetZoom();
			}

			renderBackend.DrawRect(
				destRect,
				renderData.rotation,
				renderData.color,
				renderData.filled,
				renderData.blendMode);
		}

		static std::optional<Data::RectRenderData> Cull(
			const Data::RectRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			using namespace DF2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			RectF destRect = renderData.destRect;

			auto screenPos = camera->WorldToScreen(Vector2F(destRect.x, destRect.y));

			destRect.x = screenPos.x;
			destRect.y = screenPos.y;
			destRect.w *= camera->GetZoom();
			destRect.h *= camera->GetZoom();

			if (Collision::RectVsRect(destRect, camera->GetViewBox()))
				return renderData;

			return std::nullopt;
		}
	};
}