#include "Core/SubSystems/Systems/Rendering/Pipeline/CameraRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/Pipeline/CompositeRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/Pipeline/OverlayRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/RenderPipeline.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Data;


	RenderPipeline::RenderPipeline()
	{
		passes.push_back(std::make_unique<CameraRenderPass>());
		passes.push_back(std::make_unique<CompositeRenderPass>());
		passes.push_back(std::make_unique<OverlayRenderPass>());
	}

	void RenderPipeline::Execute(IRenderBackend& renderBackend, const std::vector<RenderTask>& renderTasks)
	{
		for (auto& pass : passes)
		{
			pass->Execute(renderBackend, renderTasks);
		}
	}
}