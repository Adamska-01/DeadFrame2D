#pragma once
#include "Data/Rendering/Pipeline/RenderPhase.h"
#include "Data/Rendering/Pipeline/RenderSortKey.h"
#include "Data/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include <array>
#include <unordered_map>


namespace DeadFrame2D::Engine
{
	class Camera;
}


namespace DeadFrame2D::Core
{
	class DF2D_API RenderSystem
	{
	private:
		static DeadFrame2D::Data::RenderSortKey BuildRenderKey(int sortingOrder, uint32_t tieBreaker);


	protected:
		static std::array<
			std::unordered_map<
				DeadFrame2D::Engine::Camera*, 
				std::vector<DeadFrame2D::Data::RenderTask>>,
			(int)DeadFrame2D::Data::RenderPhase::RENDER_PHASE_COUNT> renderTasks;


	public:
		static void Submit(DeadFrame2D::Data::RenderTask renderTask);
	};
}