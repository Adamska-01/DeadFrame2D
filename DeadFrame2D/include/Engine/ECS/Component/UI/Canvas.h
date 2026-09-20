#pragma once
#include "Core/Context/Systems/UI/Context/UIContext.h"
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Canvas/CanvasRenderMode.h"
#include "Data/Components/UI/Canvas/UIScaleMode.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include <memory>
#include <string>
#include <string_view>
#include <vector>


namespace DF2D::Engine
{
	class Camera;
	class DispatchableEvent;


	/**
	 * @brief The root of a UI tree: one canvas owns one UI context and everything drawn inside it.
	 *
	 * Every UI component under a canvas contributes to the same element tree, and the whole tree is
	 * drawn by a single render task carrying the canvas's sort order. That is deliberate: the UI
	 * backend emits commands whose order is significant, so they must not be split up and re-sorted.
	 */
	class DF2D_API Canvas : public UIComponent
	{
		TYPE_INFO(Canvas, UIComponent);


	private:
		/** @brief Rendering order of this canvas relative to other canvases. */
		int sortOrder;

		Data::CanvasRenderMode renderMode;

		/**
		 * @brief Camera used when rendering in screen space camera mode.
		 *
		 * If null, the canvas behaves as if it were in screen space overlay mode.
		 */
		ComponentHandle<Camera> renderCamera;

		Data::RenderTask renderTask;

		/** @brief Stylesheets requested before the context existed, replayed once it does. */
		std::vector<std::string> pendingStyleSheets;

		Data::UIScaleMode scaleMode;

		Core::Vector2I referenceResolution;

		/** 
		 * @brief 0 scales purely off width, 1 purely off height, in between blends the two.
		 * Only meaningful under SCALE_WITH_SCREEN_SIZE.
		 */
		float matchWidthOrHeight;

		/** @brief Additional scale applied to UI dp units to convert them to pixels. */
		float uiScaleFactor;

		/** @brief The render target size used to compute uiScaleFactor. */
		Core::Vector2I targetSize;


		void RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		/** @brief Recomputes uiScaleFactor and updates the UI layout if it changes. */
		void RecomputeUIScaleFactor();


	protected:
		void OnElementCreated() override;


	public:
		Canvas();

		virtual ~Canvas() override;


		void Init() override;

		void Draw() override;


		/**
		 * @brief Records what kind of element a UI GameObject in this canvas needs.
		 *
		 * Called by UIComponent during Init;
		 */
		void DeclareElementFor(const ObjectHandle<GameObject>& owner, Data::UIElementType type);

		/**
		 * @brief Returns the element backing a UI GameObject in this canvas, creating it if needed.
		 *
		 * Called by UIComponent during Start;
		 */
		Core::UIElement AcquireElementFor(const ObjectHandle<GameObject>& owner);

		/**
		 * @brief Applies a stylesheet on top of the ones already loaded.
		 *
		 * Sheets accumulate, so a game theme layers over the engine defaults rather than replacing
		 * them, and inline properties set by components still win over both.
		 */
		bool LoadStyleSheet(std::string_view path);

		Core::UIContext GetContext() const;

		Data::CanvasRenderMode GetRenderMode() const;

		int GetSortOrder() const;

		ComponentHandle<Camera> GetRenderCamera();

		Data::UIScaleMode GetUIScaleMode() const;

		const Core::Vector2I& GetReferenceResolution() const;

		/** @brief Returns the scale applied to UI dp units when converting them to pixels. */
		float GetUIScaleFactor() const;

		void SetSortOrder(int value);

		/** @brief Sets how UI dp units scale when the render target size changes. */
		void SetUIScaleMode(Data::UIScaleMode mode);

		/** @brief Sets the resolution used as the reference for UI scaling. Ignored by CONSTANT_PIXEL_SIZE. */
		void SetReferenceResolution(const Core::Vector2I& referenceResolution);

		/** @brief Sets how much UI scaling is based on width versus height. */
		void SetMatchWidthOrHeight(float matchWidthOrHeight);

		/** 
		* @brief Renders directly in screen space, independently of any camera.
		*/
		void SetScreenSpaceOverlay();

		/**
		 * @brief Renders in screen space relative to a camera.
		 *
		 * @param targetCamera: Camera to render through; falls back to overlay mode when null.
		 */
		void SetScreenSpaceCamera(ComponentHandle<Camera> targetCamera);
	};
}