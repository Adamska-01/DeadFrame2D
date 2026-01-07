#include "Core/SubSystems/Systems/Rendering/Pipeline/CompositeRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/Renderer.h"
#include "Engine/Components/Rendering/Camera.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	void CompositeRenderPass::Execute(
		IRenderBackend& renderBackend, 
		std::array<
			std::unordered_map<
				Camera*, 
				std::vector<RenderTask>>, 
			(int)RenderPhase::RENDER_PHASE_COUNT>& renderTasks)
	{
		renderBackend.SetRenderTarget(NULL);
		renderBackend.ClearCurrentRenderTarget();

		for (auto camera : Camera::GetCameras())
		{
			if (!camera->IsActive())
				continue;

			auto viewport = camera->GetNormalizedViewBox();

			SDL_RenderCopyF(
				Renderer::GetRenderer(),
				camera->GetRenderTarget(),
				nullptr,
				&viewport);
		}
	}
}