#pragma once
#include "Core/Context/Systems/UI/Elements/Base/UIElement.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/GeometryDrawList.h"
#include "Data/Systems/UI/UIContextID.h"
#include "Data/Systems/UI/UIElementType.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <string>


namespace DF2D::Engine
{
	class Canvas;

	class GameObject;
}


namespace DF2D::Core
{
	class UIManager;


	/**
	 * @brief A handle to one UI surface: an element tree with its own styling, backing one Canvas.
	 *
	 * Held by the canvas that created it and passed down to the components under it.
	 */
	class DF2D_API UIContext
	{
		// Only the canvas that owns a surface may destroy it; everyone else holds a handle to one.
		friend class Engine::Canvas;

		friend class UIManager;


	private:
		UIManager* manager = nullptr;

		Data::UIContextID id = 0;


		UIContext(UIManager* manager, Data::UIContextID id);

		/**
		 * @brief Destroys the surface and every element in it. The handle is inert afterwards.
		 *
		 * Private on purpose: a surface outlives every handle to it except the canvas's, so letting
		 * any holder destroy it would leave the rest pointing at nothing.
		 */
		void Destroy();


	public:
		UIContext() = default;


		bool operator==(const UIContext& other) const;

		bool operator!=(const UIContext& other) const;


		bool IsValid() const;

		Data::UIContextID Id() const;


		/** @brief Resizes the surface that percentage lengths resolve against. */
		void SetSize(Vector2I size);

		/** @brief Applies a stylesheet on top of the ones already loaded, restyling everything in it. */
		bool LoadStyleSheet(const std::string& path);

		/** @brief The root element every other element in this surface hangs from. */
		UIElement GetRootElement() const;

		/** @brief Renders the surface into an ordered draw list, ready to submit as a render task. */
		Data::GeometryDrawList Render();


		/**
		 * @brief Records what kind of element a UI GameObject needs, without creating it yet.
		 *
		 * The most specific declaration wins; PANEL means no opinion.
		 */
		void DeclareElementType(const Engine::ObjectHandle<Engine::GameObject>& owningObject, Data::UIElementType type);

		/**
		 * @brief Returns the element backing a UI GameObject, creating it on first request.
		 *
		 * The element belongs to the GameObject, not to the component that asked: a single object
		 * carrying a RectTransform, an Image and a Text is one styled element, not three siblings.
		 */
		UIElement AcquireElement(const Engine::ObjectHandle<Engine::GameObject>& owningObject);

		/** @brief Drops one reference to a GameObject's element, destroying it when the last one goes. */
		void ReleaseElement(const Engine::ObjectHandle<Engine::GameObject>& owningObject);
	};
}