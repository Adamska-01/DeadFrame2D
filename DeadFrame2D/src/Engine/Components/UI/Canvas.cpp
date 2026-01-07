#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Components/UI/Canvas.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Data;


	Canvas::Canvas()
		// TODO: Create constants for sort orders
		: sortOrder(2),
		renderMode(CanvasRenderMode::SCREEN_SPACE_OVERLAY)
	{
	}

	CanvasRenderMode Canvas::GetRenderMode() const
	{
		return renderMode;
	}

	int Canvas::GetSortOrder() const
	{
		return sortOrder;
	}

	ComponentHandle<Camera> Canvas::GetRenderCamera()
	{
		return renderCamera;
	}

	void Canvas::SetSortOrder(int value)
	{
		sortOrder = value;
	}

	void Canvas::SetScreenSpaceOverlay()
	{
		renderMode = CanvasRenderMode::SCREEN_SPACE_OVERLAY;

		renderCamera = nullptr;
	}

	void Canvas::SetScreenSpaceCamera(ComponentHandle<Camera> targetCamera)
	{
		renderMode = targetCamera != nullptr
			? CanvasRenderMode::SCREEN_SPACE_CAMERA
			: CanvasRenderMode::SCREEN_SPACE_OVERLAY;

		this->renderCamera = targetCamera;
	}
}