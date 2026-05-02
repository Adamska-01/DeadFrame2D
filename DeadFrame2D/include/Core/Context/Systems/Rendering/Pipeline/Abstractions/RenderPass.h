#pragma once
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include <array>
#include <unordered_map>


namespace DF2D::Engine
{
	class Camera;
}


namespace DF2D::Core
{
	class IRenderBackend;


	class RenderPass
	{
	public:
		virtual void Execute(
			IRenderBackend& renderBackend, 
			std::array<
				std::unordered_map<
					Engine::Camera*, 
					std::vector<Data::RenderTask>>,
				(int)Data::RenderPhase::RENDER_PHASE_COUNT>& renderTasks) = 0;
	};
}