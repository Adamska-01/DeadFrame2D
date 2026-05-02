#include "Core/Context/Systems/Rendering/Pipeline/CameraRenderPass.h"
#include "Core/Context/Systems/Rendering/Pipeline/CompositeRenderPass.h"
#include "Core/Context/Systems/Rendering/Pipeline/OverlayRenderPass.h"
#include "Core/Context/Systems/Rendering/RenderPipeline.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"


namespace DF2D::Core
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Utilities;


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