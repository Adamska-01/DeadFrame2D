#include "Core/SubSystems/Systems/Rendering/RenderSystem.h"
#include "Core/SubSystems/Systems/Rendering/Resolvers/RenderResolver.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Components/UI/Canvas.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Utilities;


	std::array<std::unordered_map<Camera*, std::vector<RenderTask>>, (int)RenderPhase::RENDER_PHASE_COUNT> RenderSystem::renderTasks;


	RenderSortKey RenderSystem::BuildRenderKey(int sortOrder, uint32_t tieBreaker)
	{
		return (int64_t(sortOrder) << 32) | int64_t(tieBreaker);
	}

	void RenderSystem::Submit(RenderTask renderTask)
	{
		auto phaseIndex = static_cast<int>(renderTask.renderPhase);

		auto addRenderTask = [&](int phaseIndex, Camera* camera)
			{
				auto& bucket = renderTasks[phaseIndex][camera];

				auto& taskInBucket = bucket.emplace_back(std::move(renderTask));

				taskInBucket.renderSortKey = BuildRenderKey(
					taskInBucket.sortOrder,
					static_cast<int>(bucket.size()) - 1);
			};

		auto filterAndAddRenderTask = [&](const ComponentHandle<Camera>& cameraHandle)
			{
				std::visit([&](auto& data)
					{
						using T = std::decay_t<decltype(data)>;

						auto filtered = RenderResolver::GetRenderResolver<T>()(data, cameraHandle);

						if (!filtered)
							return;

						renderTask.renderData = std::move(*filtered);

						addRenderTask(phaseIndex, cameraHandle());
					},
					renderTask.renderData);
			};

		// Handle camera-dependent phases
		for (const auto& camera : Camera::GetCameras())
		{
			if (!camera->IsActive())
				continue;

			auto cameraHandle = camera->GetHandleAs<Camera>();

			switch (renderTask.renderPhase)
			{
			case RenderPhase::WORLD:
			case RenderPhase::DEBUG_WORLD:
				filterAndAddRenderTask(cameraHandle);
				break;

			case RenderPhase::SCREEN_SPACE_CAMERA_UI:
				if (renderTask.canvas->GetRenderCamera() == cameraHandle)
				{
					filterAndAddRenderTask(cameraHandle);
				}
				break;

			default:
				break;
			}
		}


		// Handle non-camera-dependent phases
		switch (renderTask.renderPhase)
		{
		case RenderPhase::SCREEN_SPACE_OVERLAY_UI:
		case RenderPhase::DEBUG_OVERLAY:
			addRenderTask(phaseIndex, nullptr);
			break;

		default:
			break;
		}
	}
}