#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/PointBatchRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::PointBatchRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::PointBatchRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			for (const auto& point : renderData.pointBatch)
			{
				auto pos = point.pos;

				if (camera != nullptr && requiresScreenSpaceConversion)
				{
					pos = camera->WorldToScreen(pos);
				}

				renderBackend.DrawPixel(pos, point.color);
			}
		}

		static std::optional<Data::PointBatchRenderData> Cull(
			const Data::PointBatchRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			using namespace DF2D::Data;
			using namespace DF2D::Utilities;


			if (camera == nullptr)
				return renderData;

			// Trade memory cost for CPU stability
			auto filtered = PointBatchRenderData();
			filtered.pointBatch.reserve(renderData.pointBatch.size());

			for (const auto& point : renderData.pointBatch)
			{
				auto screenPos = camera->WorldToScreen(point.pos);

				if (Collision::PointVsBox(screenPos, camera->GetViewBox()))
				{
					filtered.pointBatch.push_back(point);
				}
			}

			return filtered.pointBatch.empty() ? std::nullopt : std::optional{ std::move(filtered) };
		}
	};
}