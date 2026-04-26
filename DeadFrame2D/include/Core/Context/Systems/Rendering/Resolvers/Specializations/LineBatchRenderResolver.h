#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Rendering/Pipeline/Shapes/LineBatchRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	template<>
	struct RenderResolver<DeadFrame2D::Data::LineBatchRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::LineBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			for (const auto& line : renderData.lineBatch)
			{
				Vector2F p1 = line.p1;
				Vector2F p2 = line.p2;

				if (camera != nullptr && requiresScreenSpaceConversion)
				{
					p1 = camera->WorldToScreen(p1);
					p2 = camera->WorldToScreen(p2);
				}

				renderBackend.DrawLine(p1, p2, line.color);
			}
		}

		static std::optional<DeadFrame2D::Data::LineBatchRenderData> Cull(
			const DeadFrame2D::Data::LineBatchRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera)
		{
			using namespace DeadFrame2D::Data;
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			auto filtered = LineBatchRenderData();
			filtered.lineBatch.reserve(renderData.lineBatch.size());

			for (const auto& line : renderData.lineBatch)
			{
				auto screenP1 = camera->WorldToScreen(line.p1);
				auto screenP2 = camera->WorldToScreen(line.p2);

				if (Collision::SegmentVsRect(screenP1, screenP2, camera->GetViewBox()))
				{
					filtered.lineBatch.push_back(line);
				}
			}

			return filtered.lineBatch.empty() ? std::nullopt : std::optional{ std::move(filtered) };
		}
	};
}