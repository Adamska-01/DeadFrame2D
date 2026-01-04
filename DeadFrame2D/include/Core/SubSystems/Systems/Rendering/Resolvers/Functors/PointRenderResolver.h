#pragma once
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/PointRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Core
{
	struct PointRenderResolver
	{
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::PointRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
		{
			Vector2F pos = renderData.pos;

			if (camera != nullptr && requiresScreenSpaceConversion)
			{
				pos = camera->WorldToScreen(renderData.pos);

				if (!camera->IsVisible(pos, pos))
					return;
			}

			renderBackend.DrawPixel(pos, renderData.color);
		}
	};
}