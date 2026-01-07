#pragma once
#include "Core/SubSystems/Systems/Rendering/Pipeline/Abstractions/RenderPass.h"


namespace DeadFrame2D::Core
{
	class CompositeRenderPass : public RenderPass
	{
	public:
		void Execute(
			IRenderBackend& renderBackend, 
			std::array<
				std::unordered_map<
					DeadFrame2D::Engine::Camera*, 
					std::vector<DeadFrame2D::Data::RenderTask>>,
				(int)DeadFrame2D::Data::RenderPhase::RENDER_PHASE_COUNT>& renderTasks) override;
	};
}