#pragma once
#include "Core/SubSystems/Systems/Rendering/Pipeline/Abstractions/RenderPass.h"


namespace DeadFrame2D::Core
{
	class CompositeRenderPass : public RenderPass
	{
	public:
		void Execute(IRenderBackend& renderBackend, const std::vector<DeadFrame2D::Data::RenderTask>& renderTasks) override;
	};
}