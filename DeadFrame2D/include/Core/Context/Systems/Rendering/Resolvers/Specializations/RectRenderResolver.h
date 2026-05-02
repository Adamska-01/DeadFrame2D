#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Rendering/Pipeline/Shapes/RectRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	template<>
	struct RenderResolver<DeadFrame2D::Data::RectRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::RectRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			SDL_FRect destRect = renderData.destRect;

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
				renderData.filled);
		}

		static std::optional<DeadFrame2D::Data::RectRenderData> Cull(
			const DeadFrame2D::Data::RectRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera)
		{
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			SDL_FRect destRect = renderData.destRect;

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