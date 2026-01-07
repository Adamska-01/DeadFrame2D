#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/LineRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Collisions/CollisionUtils.h"


namespace DeadFrame2D::Core
{
	struct LineRenderResolver
	{
		// Render
		void operator()(
			IRenderBackend& renderBackend,
			const DeadFrame2D::Data::LineRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera,
			bool requiresScreenSpaceConversion) const
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

		// Visibility Check
		bool operator()(
			const DeadFrame2D::Data::LineRenderData& renderData,
			DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Camera> camera) const
		{
			using namespace DeadFrame2D::Utilities;


			// Always visible if no camera (screen space)
			if (camera == nullptr)
				return true;

			auto screenP1 = camera->WorldToScreen(renderData.p1);
			auto screenP2 = camera->WorldToScreen(renderData.p2);

			return Collision::SegmentVsRect(screenP1, screenP2, camera->GetViewBox());
		}
	};
}