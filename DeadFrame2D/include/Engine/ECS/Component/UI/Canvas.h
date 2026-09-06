#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Canvas/CanvasRenderMode.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "Data/Systems/UI/UIContextID.h"
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
		/**
		 * @brief Rendering order of this canvas relative to other canvases.
		 *
		 * Higher values are drawn on top of lower ones.
		 */
		int sortOrder;

		Data::CanvasRenderMode renderMode;

		/**
		 * @brief Camera used when rendering in screen space camera mode.
		 *
		 * If null, the canvas behaves as if it were in screen space overlay mode.
		 */
		ComponentHandle<Camera> renderCamera;

		Data::UIContextID context = 0;

		Data::RenderTask renderTask;

		/** @brief Stylesheets requested before the context existed, replayed once it does. */
		std::vector<std::string> pendingStyleSheets;


		void RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	protected:
		void OnElementCreated() override;


	public:
		Canvas();

		virtual ~Canvas() override;


		void Init() override;

		void Draw() override;


		/**
		 * @brief Returns the element backing a UI GameObject in this canvas, creating it if needed.
		 *
		 * Called by UIComponent during its own Init; there is no reason for game code to call it.
		 */
		Data::UIElementID AcquireElementFor(const ObjectHandle<GameObject>& owner, Data::UIElementType type);

		/**
		 * @brief Applies a stylesheet on top of the ones already loaded.
		 *
		 * Sheets accumulate, so a game theme layers over the engine defaults rather than replacing
		 * them, and inline properties set by components still win over both.
		 */
		bool LoadStyleSheet(std::string_view path);

		Data::UIContextID GetContext() const;

		Data::CanvasRenderMode GetRenderMode() const;

		int GetSortOrder() const;

		ComponentHandle<Camera> GetRenderCamera();

		void SetSortOrder(int value);

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