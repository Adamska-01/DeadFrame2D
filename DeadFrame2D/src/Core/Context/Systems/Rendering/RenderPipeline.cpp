#include "Core/Context/Systems/Rendering/Pipeline/CameraRenderPass.h"
#include "Core/Context/Systems/Rendering/Pipeline/CompositeRenderPass.h"
#include "Core/Context/Systems/Rendering/Pipeline/OverlayRenderPass.h"
#include "Core/Context/Systems/Rendering/RenderPipeline.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Utilities;


	RenderPipeline::RenderPipeline()
	{
		passes.push_back(std::make_unique<CameraRenderPass>());
		passes.push_back(std::make_unique<CompositeRenderPass>());
		passes.push_back(std::make_unique<OverlayRenderPass>());
	}

	void RenderPipeline::Execute(
		IRenderBackend& renderBackend, 
		std::array<
			std::unordered_map<
				Camera*, 
				std::vector<RenderTask>>,
			(int)RenderPhase::RENDER_PHASE_COUNT>& renderTasks)
	{
		for (auto& pass : passes)
		{
			pass->Execute(renderBackend, renderTasks);
		}
	}
}