#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/PointBatchRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	struct PointBatchRenderResolver
	{
		// Render
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::PointBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
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

		// Visibility Check
		std::optional<DeadFrame2D::Data::PointBatchRenderData> operator()(
			const DeadFrame2D::Data::PointBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera) const
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