#pragma once
#include "Data/Rendering/Pipeline/RenderData.h"
#include "Data/Rendering/Pipeline/RenderPhase.h"
#include "Data/Rendering/Pipeline/RenderSortKey.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Engine
{
	class Canvas;
}


namespace DeadFrame2D::Data
{
	struct RenderTask
	{
		friend class RenderSystem;


	private:
		RenderSortKey renderSortKey;


	public:
		RenderPhase renderPhase;

		RenderData renderData;

		int sortOrder;

		DeadFrame2D::Engine::ComponentHandle<DeadFrame2D::Engine::Canvas> canvas;


		inline uint64_t GetSortKey() const
		{
			return renderSortKey;
		}
	};
}