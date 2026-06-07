#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Circle.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/CircleRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::CircleRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::CircleRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			auto pos = renderData.center;
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


		static std::optional<Data::CircleRenderData> Cull(
			const Data::CircleRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			using namespace DF2D::Utilities;


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