#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Pipeline/CompositeRenderPass.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"


namespace DF2D::Core
{
	using namespace DF2D::Engine;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	void CompositeRenderPass::Execute(
		IRenderBackend& renderBackend,
		std::array<
			std::unordered_map<
				Camera*,
				std::vector<RenderTask>>,
			(int)RenderPhase::RENDER_PHASE_COUNT>& renderTasks)
	{
		renderBackend.SetRenderTarget(0);
		renderBackend.ClearCurrentRenderTarget();

		for (auto camera : Camera::GetCameras())
		{
			if (!camera->IsActive())
				continue;

			auto viewport = camera->GetNormalizedViewBox();

			renderBackend.DrawTexture(
				camera->GetRenderTarget(),
				std::nullopt,
				viewport,
				std::nullopt,
				0.0f,
				Data::RenderFlip::NONE,
				Constants::CommonColors::WHITE);
		}
	}
}