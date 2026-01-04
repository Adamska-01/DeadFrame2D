#include "Core/SubSystems/Systems/Rendering/Pipeline/CompositeRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/Renderer.h"
#include "Engine/Components/Rendering/Camera.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;


	void CompositeRenderPass::Execute(IRenderBackend& renderBackend, const std::vector<RenderTask>& renderTasks)
	{
		renderBackend.SetRenderTarget(NULL);
		renderBackend.ClearCurrentRenderTarget();

		for (auto camera : Camera::GetCameras())
		{
			if (!camera->IsActive())
				continue;

			auto viewport = camera->GetNormalizedViewBox();

			SDL_RenderCopy(
				Renderer::GetRenderer(),
				camera->GetRenderTarget(),
				nullptr,
				&viewport);
		}
	}
}