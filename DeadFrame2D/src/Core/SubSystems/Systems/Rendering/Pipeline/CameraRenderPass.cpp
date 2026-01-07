#include "Core/SubSystems/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/SubSystems/Systems/Rendering/Pipeline/CameraRenderPass.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/RenderResolver.h"
#include "Engine/Components/Rendering/Camera.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	CameraRenderPass::CameraRenderPass()
		: phasesInOrder{
			RenderPhase::WORLD,
			RenderPhase::DEBUG_WORLD,
			RenderPhase::SCREEN_SPACE_CAMERA_UI
		}
	{
	}

	void CameraRenderPass::Execute(
		IRenderBackend& renderBackend, 
		std::array<
			std::unordered_map<
				Camera*, 
				std::vector<RenderTask>>, 
			(int)RenderPhase::RENDER_PHASE_COUNT>& renderTasks)
	{
		for (auto camera : Camera::GetCameras())
		{
			renderBackend.SetRenderTarget(camera->GetRenderTarget());

			auto cameraHandle = camera->GetHandleAs<Camera>();

			for (auto phase : phasesInOrder)
			{
				auto key = static_cast<int>(phase);

				auto phaseTasksIT = renderTasks[key].find(camera);

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

							RenderResolver::GetRenderResolver<T>()(
								renderBackend,
								data,
								cameraHandle,
								task.renderPhase != RenderPhase::SCREEN_SPACE_CAMERA_UI);
						},
						task.renderData);
				}
			}
		}
	}
}