#pragma once
#include "Core/SubSystems/Systems/Rendering/Pipeline/Abstractions/RenderPass.h"
#include "Data/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include <vector>


namespace DeadFrame2D::Core
{
	class IRenderBackend;


	class DF2D_API RenderPipeline
	{
	private:
		std::vector<std::unique_ptr<RenderPass>> passes;


	public:
		RenderPipeline();


		void Execute(IRenderBackend& renderBackend, const std::vector<DeadFrame2D::Data::RenderTask>& renderTasks);
	};
}