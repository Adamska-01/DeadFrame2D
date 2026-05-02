#pragma once
#include "Core/Context/Systems/Rendering/Pipeline/Abstractions/RenderPass.h"


namespace DF2D::Core
{
	class CompositeRenderPass : public RenderPass
	{
	public:
		void Execute(
			IRenderBackend& renderBackend, 
			std::array<
				std::unordered_map<
					Engine::Camera*, 
					std::vector<Data::RenderTask>>,
				(int)Data::RenderPhase::RENDER_PHASE_COUNT>& renderTasks) override;
	};
}