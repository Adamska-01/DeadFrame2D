#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Circle.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/CircleBatchRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::CircleBatchRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::CircleBatchRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			for (const auto& circle : renderData.circleBatch)
			{
				auto pos = circle.center;
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
					circle.filled,
					circle.blendMode);
			}
		}

		static std::optional<Data::CircleBatchRenderData> Cull(
			const Data::CircleBatchRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			using namespace DF2D::Data;
			using namespace DF2D::Utilities;


			if (camera == nullptr)
				return renderData;

			// Trade memory cost for CPU stability
			auto filtered = CircleBatchRenderData();
			filtered.circleBatch.reserve(renderData.circleBatch.size());

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