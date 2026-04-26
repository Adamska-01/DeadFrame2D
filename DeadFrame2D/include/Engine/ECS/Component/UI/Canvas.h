#pragma once
#include "Data/Rendering/CanvasRenderMode.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Abstractions/UIComponent.h"


namespace DeadFrame2D::Engine
{
	class Camera;


	class DF2D_API Canvas : public UIComponent
	{
		TYPE_INFO(Canvas, UIComponent);


	private:
		/**
		 * @brief Determines the rendering order of the canvas relative to other canvases.
		 *
		 * Higher values are rendered on top of lower values.
		 */
		int sortOrder;

		/**
		 * @brief Current render mode used by the canvas.
		 */
		DeadFrame2D::Data::CanvasRenderMode renderMode;

		/**
		 * @brief Target camera used when rendering in screen space camera mode.
		 *
		 * If null, the canvas behaves as if it were in screen space overlay mode.
		 */
		ComponentHandle<Camera> renderCamera;


	public:
		Canvas();


		/**
		 * @brief Returns the current render mode of the canvas.
		 *
		 * @return The active canvas render mode.
		 */
		DeadFrame2D::Data::CanvasRenderMode GetRenderMode() const;

		/**
		 * @brief Returns the sorting order of the canvas.
		 *
		 * @return The canvas sort order.
		 */
		int GetSortOrder() const;

		/**
		 * @brief Returns the render camera set to the canvas.
		 *
		 * @return The canvas render camera.
		 */
		ComponentHandle<Camera> GetRenderCamera();

		/**
		 * @brief Sets the sorting order of the canvas.
		 *
		 * @param value: New sort order value.
		 */
		void SetSortOrder(int value);

		/**
		 * @brief Sets the canvas to render directly in screen space overlay mode.
		 *
		 * Rendering is independent of any camera.
		 */
		void SetScreenSpaceOverlay();

		/**
		 * @brief Sets the canvas to render in screen space relative to a camera.
		 *
		 * @param targetCamera: Camera used for rendering. (if null, it will fallback 
		 * to screen space overlay mode)
		 */
		void SetScreenSpaceCamera(ComponentHandle<Camera> targetCamera);
	};
}