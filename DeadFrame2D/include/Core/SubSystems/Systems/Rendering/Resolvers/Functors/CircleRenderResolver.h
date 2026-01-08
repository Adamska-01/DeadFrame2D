#pragma once
#include "Core/Math/Circle.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/CircleRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	struct CircleRenderResolver
	{
		// Render
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::CircleRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
		{
			Vector2F pos = renderData.center;
			auto radius = renderData.radius;

			if (camera != nullptr && requiresScreenSpaceConversion)
			{
				pos = camera->WorldToScreen(renderData.center);
				radius = renderData.radius * camera->GetZoom();
			}

			renderBackend.DrawCircle(
				pos,
				radius,
				renderData.color,
				renderData.filled);
		}


		// Visibility Check
		std::optional<DeadFrame2D::Data::CircleRenderData> operator()(
			const DeadFrame2D::Data::CircleRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera) const
		{
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			auto circle = Circle(
				camera->WorldToScreen(renderData.center),
				renderData.radius * camera->GetZoom());

			if (Collision::CircleVsRect(circle, camera->GetViewBox()))
				return renderData;

			return std::nullopt;
		}
	};
}