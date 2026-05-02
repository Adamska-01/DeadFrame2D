#pragma once
#include "Core/Context/Systems/Rendering/Pipeline/Abstractions/RenderPass.h"
#include "Data/Systems/Rendering/Pipeline/RenderPhase.h"


namespace DeadFrame2D::Core
{
	class CameraRenderPass : public RenderPass
	{
	private:
		std::array<DeadFrame2D::Data::RenderPhase, 3> phasesInOrder;


	public:
		CameraRenderPass();


		void Execute(
			IRenderBackend& renderBackend, 
			std::array<
				std::unordered_map<
					DeadFrame2D::Engine::Camera*, 
					std::vector<DeadFrame2D::Data::RenderTask>>,
				(int)DeadFrame2D::Data::RenderPhase::RENDER_PHASE_COUNT>& renderTasks) override;
	};
}