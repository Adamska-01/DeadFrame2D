#pragma once
#include "Data/Rendering/Pipeline/RenderData.h"
#include "Data/Rendering/Pipeline/RenderPhase.h"
#include "Data/Rendering/Pipeline/RenderSortKey.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Core
{
	class RenderSystem;
}

namespace DeadFrame2D::Engine
{
	class Canvas;
}


namespace DeadFrame2D::Data
{
	struct RenderTask
	{
		friend class DeadFrame2D::Core::RenderSystem;


	private:
		RenderSortKey renderSortKey;


	public:
		RenderTask() = default;

		RenderTask(const RenderTask&) = default;

		RenderTask(RenderTask&&) noexcept = default;

		RenderTask& operator=(const RenderTask&) = default;

		RenderTask& operator=(RenderTask&&) noexcept = default;


		RenderPhase renderPhase;

		RenderData renderData;

		int sortOrder;

		DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Canvas> canvas;


		inline RenderSortKey GetSortKey() const
		{
			return renderSortKey;
		}
	};
}