#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/SpriteRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Core
{
	struct SpriteRenderResolver
	{
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::SpriteRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
		{
			SDL_FRect destRect = *renderData.destRect;

			if (camera != nullptr && requiresScreenSpaceConversion && renderData.destRect)
			{
				auto screenPos = camera->WorldToScreen(Vector2F(renderData.destRect->x, renderData.destRect->y));

				destRect.x = screenPos.x;
				destRect.y = screenPos.y;
				destRect.w *= camera->GetZoom();
				destRect.h *= camera->GetZoom();

				if (!camera->IsVisible(destRect))
					return;
			}

			renderBackend.DrawTexture(
				renderData.texture,
				renderData.srcRect,
				&destRect,
				&renderData.rotationOrigin,
				renderData.rotation,
				renderData.flip,
				renderData.colorMod);
		}
	};
}