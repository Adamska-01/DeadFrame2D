#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/SubSystems/Systems/Rendering/Pipeline/CameraRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/RenderResolver.h"
#include "Data/Rendering/Pipeline/RenderPhase.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Components/UI/Canvas.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;


	void CameraRenderPass::Execute(IRenderBackend& renderBackend, const std::vector<RenderTask>& renderTasks)
	{
		for (auto camera : Camera::GetCameras())
		{
			if (!camera->IsActive())
				continue;

			auto cameraHandle = camera->GetHandleAs<Camera>();

			renderBackend.SetRenderTarget(camera->GetRenderTarget());

			for (auto& task : renderTasks)
			{
				// All the WORLD ones are guaranteed come before SCREEN_SPACE_CAMERA_UI thanks to the sorting, so it's fine to do it in a single loop.
				switch (task.renderPhase)
				{
				case RenderPhase::WORLD:
				case RenderPhase::DEBUG_WORLD:
					std::visit(
						[&](const auto& data)
						{
							using T = std::decay_t<decltype(data)>;

							RenderResolver::GetRenderResolver<T>()(
								renderBackend,
								data,
								cameraHandle,
								true);
						},
						task.renderData);
					break;

				case RenderPhase::SCREEN_SPACE_CAMERA_UI:
					if (task.canvas->GetRenderCamera() == cameraHandle)
					{
						std::visit(
							[&](const auto& data)
							{
								using T = std::decay_t<decltype(data)>;

								RenderResolver::GetRenderResolver<T>()(
									renderBackend,
									data,
									cameraHandle,
									false);
							},
							task.renderData);
					}
					break;

				default:
					break;
				}
			}
		}
	}
}