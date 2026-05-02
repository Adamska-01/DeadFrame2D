#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/PointBatchRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	template<>
	struct RenderResolver<DeadFrame2D::Data::PointBatchRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::PointBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			for (const auto& point : renderData.pointBatch)
			{
				Vector2F pos = point.pos;

				if (camera != nullptr && requiresScreenSpaceConversion)
				{
					pos = camera->WorldToScreen(pos);
				}

				renderBackend.DrawPixel(pos, point.color);
			}
		}

		static std::optional<DeadFrame2D::Data::PointBatchRenderData> Cull(
			const DeadFrame2D::Data::PointBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera)
		{
			using namespace DeadFrame2D::Data;
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			auto filtered = PointBatchRenderData();
			filtered.pointBatch.reserve(renderData.pointBatch.size()); // Trade memory cost for CPU stability

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