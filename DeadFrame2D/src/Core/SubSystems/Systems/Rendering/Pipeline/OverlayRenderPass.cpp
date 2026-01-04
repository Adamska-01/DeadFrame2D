#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/SubSystems/Systems/Rendering/Pipeline/OverlayRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/RenderResolver.h"
#include "Data/Rendering/Pipeline/RenderPhase.h"
#include "Engine/Components/Rendering/Camera.h"
#include <variant>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;


	void OverlayRenderPass::Execute(IRenderBackend& renderBackend, const std::vector<RenderTask>& renderTasks)
	{
		renderBackend.SetRenderTarget(NULL);
		renderBackend.ClearCurrentRenderTarget();

		for (auto camera : Camera::GetCameras())
		{
			auto cameraHandle = camera->GetHandleAs<Camera>();

			for (auto& task : renderTasks)
			{
				switch (task.renderPhase)
				{
				case RenderPhase::SCREEN_SPACE_OVERLAY_UI:
				case RenderPhase::DEBUG_OVERLAY:
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