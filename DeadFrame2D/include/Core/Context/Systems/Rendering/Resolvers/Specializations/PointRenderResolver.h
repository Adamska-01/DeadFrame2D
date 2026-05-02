#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/PointRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::PointRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::PointRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			Vector2F pos = renderData.pos;

			if (camera != nullptr && requiresScreenSpaceConversion)
			{
				pos = camera->WorldToScreen(renderData.pos);
			}

			renderBackend.DrawPixel(pos, renderData.color);
		}

		static std::optional<Data::PointRenderData> Cull(
			const Data::PointRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			using namespace DF2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return renderData;

			auto pos = camera->WorldToScreen(renderData.pos);

			if (Collision::PointVsBox(pos, camera->GetViewBox()))
				return renderData;

			return std::nullopt;
		}
	};
}