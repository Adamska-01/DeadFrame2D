#pragma once
#include "Data/Rendering/Pipeline/RenderPhase.h"
#include "Data/Rendering/Pipeline/RenderSortKey.h"
#include "Data/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include <vector>


namespace DeadFrame2D::Core
{
	class DF2D_API RenderSystem
	{
	private:
		static DeadFrame2D::Data::RenderSortKey BuildRenderKey(DeadFrame2D::Data::RenderPhase phase, int sortingOrder, uint32_t tieBreaker);
	
	
	protected:
		static std::vector<DeadFrame2D::Data::RenderTask> renderTasks;


	public:
		static void Submit(DeadFrame2D::Data::RenderTask renderTask);
	};
}