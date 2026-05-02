#pragma once
#include "Data/Systems/Rendering/Pipeline/RenderData.h"
#include "Data/Systems/Rendering/Pipeline/RenderPhase.h"
#include "Data/Systems/Rendering/Pipeline/RenderSortKey.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"


namespace DF2D::Core
{
	class RenderSystem;
}

namespace DF2D::Engine
{
	class Canvas;
}


namespace DF2D::Data
{
	struct RenderTask
	{
		friend class Core::RenderSystem;


	private:
		RenderSortKey renderSortKey = 0;


	public:
		RenderTask() = default;

		RenderTask(const RenderTask&) = default;

		RenderTask(RenderTask&&) noexcept = default;

		RenderTask& operator=(const RenderTask&) = default;

		RenderTask& operator=(RenderTask&&) noexcept = default;


		RenderPhase renderPhase = RenderPhase::WORLD;

		RenderData renderData;

		int sortOrder = 0;

		Engine::ComponentHandle<Engine::Canvas> canvas;


		inline RenderSortKey GetSortKey() const
		{
			return renderSortKey;
		}
	};
}