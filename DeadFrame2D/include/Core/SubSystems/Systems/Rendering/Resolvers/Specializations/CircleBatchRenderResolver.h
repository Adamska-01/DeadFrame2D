#pragma once
#include "Core/Math/Circle.h"
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Data/Rendering/Pipeline/Shapes/CircleBatchRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	template<>
	struct RenderResolver<DeadFrame2D::Data::CircleBatchRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::CircleBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			for (const auto& circle : renderData.circleBatch)
			{
				Vector2F pos = circle.center;
				auto radius = circle.radius;

				if (camera != nullptr && requiresScreenSpaceConversion)
				{
					pos = camera->WorldToScreen(pos);
					radius *= camera->GetZoom();
				}

				renderBackend.DrawCircle(
					pos,
					radius,
					circle.color,
					circle.filled);
			}
		}

		static std::optional<DeadFrame2D::Data::CircleBatchRenderData> Cull(
			const DeadFrame2D::Data::CircleBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera)
		{
			using namespace DeadFrame2D::Data;
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			auto filtered = CircleBatchRenderData();
			filtered.circleBatch.reserve(renderData.circleBatch.size()); // Trade memory cost for CPU stability

			for (const auto& circle : renderData.circleBatch)
			{
				auto screenCircle = Circle(
					camera->WorldToScreen(circle.center),
					circle.radius * camera->GetZoom());

				if (Collision::CircleVsRect(screenCircle, camera->GetViewBox()))
				{
					filtered.circleBatch.push_back(circle);
				}
			}

			return filtered.circleBatch.empty() ? std::nullopt : std::optional{ std::move(filtered) };
		}
	};
}