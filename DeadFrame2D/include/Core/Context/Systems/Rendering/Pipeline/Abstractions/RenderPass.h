#pragma once
#include "Data/Rendering/Pipeline/RenderTask.h"
#include <array>
#include <unordered_map>


namespace DeadFrame2D::Engine
{
	class Camera;
}


namespace DeadFrame2D::Core
{
	class IRenderBackend;


	class RenderPass
	{
	public:
		virtual void Execute(
			IRenderBackend& renderBackend, 
			std::array<
				std::unordered_map<
					DeadFrame2D::Engine::Camera*, 
					std::vector<DeadFrame2D::Data::RenderTask>>,
				(int)DeadFrame2D::Data::RenderPhase::RENDER_PHASE_COUNT>& renderTasks) = 0;
	};
}