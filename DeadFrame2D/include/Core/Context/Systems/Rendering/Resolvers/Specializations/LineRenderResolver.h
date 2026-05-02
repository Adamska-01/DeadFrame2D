#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/LineRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DeadFrame2D::Core
{
	template<>
	struct RenderResolver<DeadFrame2D::Data::LineRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::LineRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			Vector2F p1 = renderData.p1;
			Vector2F p2 = renderData.p2;

			if (camera != nullptr && requiresScreenSpaceConversion)
			{
				p1 = camera->WorldToScreen(renderData.p1);
				p2 = camera->WorldToScreen(renderData.p2);
			}

			renderBackend.DrawLine(p1, p2, renderData.color);
		}

		static std::optional<DeadFrame2D::Data::LineRenderData> Cull(
			const DeadFrame2D::Data::LineRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera)
		{
			using namespace DeadFrame2D::Data;
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			auto screenP1 = camera->WorldToScreen(renderData.p1);
			auto screenP2 = camera->WorldToScreen(renderData.p2);

			if (Collision::SegmentVsRect(screenP1, screenP2, camera->GetViewBox()))
				return renderData;

			return std::nullopt;
		}
	};
}