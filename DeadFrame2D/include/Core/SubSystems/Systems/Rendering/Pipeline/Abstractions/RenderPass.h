#pragma once
#include "Data/Rendering/Pipeline/RenderTask.h"
#include <vector>


namespace DeadFrame2D::Core
{
	class IRenderBackend;


	class RenderPass
	{
	public:
		virtual void Execute(IRenderBackend& renderBackend, const std::vector<DeadFrame2D::Data::RenderTask>& renderTasks) = 0;
	};
}