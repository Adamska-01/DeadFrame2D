#include "Core/SubSystems/Systems/Rendering/RenderSystem.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Data;


	RenderSortKey RenderSystem::BuildRenderKey(RenderPhase phase, int sortOrder, uint32_t tieBreaker)
	{
		return (uint64_t(phase) << 56)
			| (uint64_t(sortOrder) << 32)
			| uint64_t(tieBreaker);
	}


	void RenderSystem::Submit(RenderTask renderTask)
	{
		renderTask.renderSortKey = BuildRenderKey(
			renderTask.renderPhase, 
			renderTask.sortOrder, 
			renderTasks.size());

		renderTasks.push_back(renderTask);
	}
}