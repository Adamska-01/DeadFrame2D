#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/SubSystems/Systems/Rendering/Pipeline/OverlayRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/RenderResolvers.h"
#include <variant>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	OverlayRenderPass::OverlayRenderPass()
		: phasesInOrder{ 
			RenderPhase::SCREEN_SPACE_OVERLAY_UI, 
			RenderPhase::DEBUG_OVERLAY 
		}
	{
	}

	void OverlayRenderPass::Execute(
		IRenderBackend& renderBackend, 
		std::array<
			std::unordered_map<
				Camera*, 
				std::vector<RenderTask>>, 
			(int)RenderPhase::RENDER_PHASE_COUNT>& renderTasks)
	{
		renderBackend.SetRenderTarget(NULL);

		for (auto phase : phasesInOrder)
		{
			auto key = static_cast<int>(phase);

			auto phaseTasksIT = renderTasks[key].find(nullptr);

			if (phaseTasksIT == renderTasks[key].end())
				continue;

			auto& tasks = phaseTasksIT->second;

			std::sort(
				tasks.begin(),
				tasks.end(),
				[](const RenderTask& a, const RenderTask& b)
				{
					return a.GetSortKey() < b.GetSortKey();
				});

			for (auto& task : tasks)
			{
				std::visit(
					[&](const auto& data)
					{
						using T = std::decay_t<decltype(data)>;

						RenderResolver<T>::Render(
							renderBackend,
							data,
							{},
							false);
					},
					task.renderData);
			}
		}
	}
}