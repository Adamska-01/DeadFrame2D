#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Data/Rendering/Pipeline/Shapes/LineRenderData.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Core
{
	struct LineRenderResolver
	{
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

				if (!camera->IsVisible(p1, p2))
					return;
			}

			renderBackend.DrawLine(p1, p2, renderData.color);
		}
	};
}