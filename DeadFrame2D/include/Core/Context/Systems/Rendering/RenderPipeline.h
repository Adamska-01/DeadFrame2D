#pragma once
#include "Core/Context/Systems/Rendering/Pipeline/Abstractions/RenderPass.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include <array>
#include <unordered_map>
#include <vector>


namespace DF2D::Engine
{
	class Camera;
}


namespace DF2D::Core
{
	class IRenderBackend;


	class DF2D_API RenderPipeline
	{
	private:
		std::vector<std::unique_ptr<RenderPass>> passes;


	public:
		RenderPipeline();

		RenderPipeline(const RenderPipeline&) = delete;

		RenderPipeline& operator=(const RenderPipeline&) = delete;

		RenderPipeline(RenderPipeline&&) noexcept = default;

		RenderPipeline& operator=(RenderPipeline&&) noexcept = default;


		void Execute(
			IRenderBackend& renderBackend, 
			std::array<
				std::unordered_map<
					Engine::Camera*, 
					std::vector<Data::RenderTask>>,
				(int)Data::RenderPhase::RENDER_PHASE_COUNT>& renderTasks);
	};
}