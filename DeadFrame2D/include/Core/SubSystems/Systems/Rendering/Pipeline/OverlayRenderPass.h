#pragma once
#include "Core/SubSystems/Systems/Rendering/Pipeline/Abstractions/RenderPass.h"
#include "Data/Rendering/Pipeline/RenderPhase.h"
#include <array>


namespace DeadFrame2D::Core
{
	class OverlayRenderPass : public RenderPass
	{
	private:
		std::array<DeadFrame2D::Data::RenderPhase, 2> phasesInOrder;


	public:
		OverlayRenderPass();


		void Execute(
			IRenderBackend& renderBackend, 
			std::array<
				std::unordered_map<
					DeadFrame2D::Engine::Camera*, 
					std::vector<DeadFrame2D::Data::RenderTask>>,
				(int)DeadFrame2D::Data::RenderPhase::RENDER_PHASE_COUNT>& renderTasks) override;
	};
}