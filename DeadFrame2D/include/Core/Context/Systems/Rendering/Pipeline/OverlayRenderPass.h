#pragma once
#include "Core/Context/Systems/Rendering/Pipeline/Abstractions/RenderPass.h"
#include "Data/Systems/Rendering/Pipeline/RenderPhase.h"
#include <array>


namespace DF2D::Core
{
	class OverlayRenderPass : public RenderPass
	{
	private:
		std::array<Data::RenderPhase, 2> phasesInOrder;


	public:
		OverlayRenderPass();


		void Execute(
			IRenderBackend& renderBackend, 
			std::array<
				std::unordered_map<
					Engine::Camera*, 
					std::vector<Data::RenderTask>>,
				(int)Data::RenderPhase::RENDER_PHASE_COUNT>& renderTasks) override;
	};
}