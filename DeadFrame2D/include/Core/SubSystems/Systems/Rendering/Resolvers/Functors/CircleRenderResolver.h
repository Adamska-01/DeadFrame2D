#pragma once
#include "Core/Math/Circle.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/CircleRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Core
{
	struct CircleRenderResolver
	{
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::CircleRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
		{
			if (camera != nullptr && requiresScreenSpaceConversion)
			{
				auto circle = Circle(
					camera->WorldToScreen(renderData.center),
					renderData.radius * camera->GetZoom());

				if (!camera->IsVisible(circle))
					return;
			}

			renderBackend.DrawCircle(
				renderData.center,
				renderData.radius,
				renderData.color,
				renderData.filled);
		}
	};
}