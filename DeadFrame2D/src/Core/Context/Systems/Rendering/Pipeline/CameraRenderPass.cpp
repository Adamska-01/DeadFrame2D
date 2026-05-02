#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Core/Context/Systems/Rendering/Pipeline/CameraRenderPass.h"
#include "Core/Context/Systems/Rendering/Resolvers/RenderResolvers.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"


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

							RenderResolver<T>::Render(
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