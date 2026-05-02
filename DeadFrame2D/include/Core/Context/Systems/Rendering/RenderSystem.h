#pragma once
#include "Data/Systems/Rendering/Pipeline/RenderPhase.h"
#include "Data/Systems/Rendering/Pipeline/RenderSortKey.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include <array>
#include <unordered_map>


namespace DF2D::Engine
{
	class Camera;
}


namespace DF2D::Core
{
	class DF2D_API RenderSystem
	{
	private:
		static Data::RenderSortKey BuildRenderKey(int sortingOrder, uint32_t tieBreaker);


	protected:
		static std::array<
			std::unordered_map<
				Engine::Camera*, 
				std::vector<Data::RenderTask>>,
			(int)Data::RenderPhase::RENDER_PHASE_COUNT> renderTasks;


	public:
		static void Submit(Data::RenderTask renderTask);
	};
}