#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/LineRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::LineRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::LineRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			auto p1 = renderData.p1;
			auto p2 = renderData.p2;

			if (camera != nullptr && requiresScreenSpaceConversion)
			{
				p1 = camera->WorldToScreen(renderData.p1);
				p2 = camera->WorldToScreen(renderData.p2);
			}

			renderBackend.DrawLine(p1, p2, renderData.color, renderData.blendMode);
		}

		static std::optional<Data::LineRenderData> Cull(
			const Data::LineRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			using namespace DF2D::Data;
			using namespace DF2D::Utilities;


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