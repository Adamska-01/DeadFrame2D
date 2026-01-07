#pragma once
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/PointRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"


namespace DeadFrame2D::Core
{
	struct PointRenderResolver
	{
		// Render
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::PointRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
		{
			Vector2F pos = renderData.pos;

			if (camera != nullptr && requiresScreenSpaceConversion)
			{
				pos = camera->WorldToScreen(renderData.pos);
			}

			renderBackend.DrawPixel(pos, renderData.color);
		}

		// Visibility Check
		bool operator()(
			const DeadFrame2D::Data::PointRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera) const
		{
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return true;

			auto pos = camera->WorldToScreen(renderData.pos);

			return Collision::PointVsBox(pos, camera->GetViewBox());
		}
	};
}