#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/RectRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"


namespace DeadFrame2D::Core
{
	struct RectRenderResolver
	{
		// Render
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::RectRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
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

		// Visibility Check
		bool operator()(
			const DeadFrame2D::Data::RectRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera) const
		{
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return true;

			SDL_FRect destRect = renderData.destRect;

			auto screenPos = camera->WorldToScreen(Vector2F(destRect.x, destRect.y));

			destRect.x = screenPos.x;
			destRect.y = screenPos.y;
			destRect.w *= camera->GetZoom();
			destRect.h *= camera->GetZoom();

			return Collision::RectVsRect(destRect, camera->GetViewBox());
		}
	};
}