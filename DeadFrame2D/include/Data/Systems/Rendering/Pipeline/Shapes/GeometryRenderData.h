#pragma once
#include "Data/Systems/Rendering/Pipeline/GeometryDrawList.h"
#include <memory>


namespace DF2D::Data
{
	/**
	 * @brief Submits a whole GeometryDrawList through the pipeline as a single render task.
	 *
	 * The list is shared rather than copied because RenderSystem::Submit takes a RenderTask by value
	 * and fans it out per camera, and the pipeline then sorts vectors of those tasks. Copying a full
	 * vertex buffer through that would be the wrong cost, so there is one owner and many cheap views.
	 *
	 * A whole list travels as one task on purpose: splitting it into a task per command would let the
	 * pipeline's sort key reorder commands that must stay in submission order.
	 */
	struct GeometryRenderData
	{
		std::shared_ptr<const GeometryDrawList> drawList;
	};
}