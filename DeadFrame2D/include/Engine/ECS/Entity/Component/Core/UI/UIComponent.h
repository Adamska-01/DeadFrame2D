#pragma once
#include "Core/Context/Systems/UI/UIContext.h"
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/UI/UIAttribute.h"
#include "Data/Systems/UI/UIElementID.h"
#include "Data/Systems/UI/UIElementType.h"
#include "Data/Systems/UI/UIEventPayload.h"
#include "Data/Systems/UI/UIEventType.h"
#include "Data/Systems/UI/UIPseudoClass.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <string>
#include <string_view>
#include <unordered_set>


namespace DF2D::Engine
{
	class Canvas;


	/**
	 * @brief Base for every UI component: one component owns one element in its canvas.
	 *
	 * The element tree mirrors the GameObject tree. A component's element is parented to the nearest
	 * ancestor GameObject that also carries a UIComponent, so reparenting a GameObject reparents its
	 * UI, and a UI object must live under a Canvas to exist at all.
	 *
	 * Derived components describe themselves by setting style properties and let the UI backend do the
	 * layout. They never compute the position or size of a child.
	 */
	class DF2D_API UIComponent : public GameComponent
	{
		TYPE_INFO(UIComponent, GameComponent);

		friend class Core::UIManager;


	private:
		bool visible = true;


		/** @brief Re-parents this element under whichever UI ancestor the GameObject now sits below. */
		void SyncElementParent();

		void OnParentGameObjectChangedHandler(const ObjectHandle<GameObject>& obj) override;

		void OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool activeState) override;


	protected:
		Core::UIContext context;

		Core::UIElement element;

		ComponentHandle<Canvas> parentCanvas;

		ComponentHandle<UIComponent> selfUIHandle;

		/** @brief Classes requested before the element existed, replayed once it does. */
		std::unordered_set<std::string> pendingClasses;


		/** @brief The element kind this component needs. Called once, during Init. */
		virtual Data::UIElementType GetElementType() const;

		/**
		 * @brief Called after the element exists and has been parented, for one-time setup.
		 *
		 * Derived components use this instead of Init so they never have to repeat the element
		 * creation and canvas resolution that every UI component needs.
		 */
		virtual void OnElementCreated();

		/** @brief Delivers an event the backend raised on this component's element. */
		virtual void HandleUIEvent(Data::UIEventType eventType, const Data::UIEventPayload& payload);


		/**
		 * @brief Sets a non-style attribute on this component's own element.
		 *
		 * Element operations go through the UI manager rather than the backend: components never hold
		 * the UI library, and never address an element other than their own.
		 */
		void SetAttribute(Data::UIAttribute attribute, const std::string& value);

		/** @brief Drops an attribute, for the ones whose presence alone is the state. */
		void RemoveAttribute(Data::UIAttribute attribute);

		/** @brief Replaces this element's text content. */
		void SetElementText(const std::string& text);

		void SetStyle(Data::UIStyleProperty property, const std::string& value);

		void SetStyle(Data::UIStyleProperty property, float pixels);

		void SetStyle(Data::UIStyleProperty property, const Core::Color& color);

		void ClearStyle(Data::UIStyleProperty property);


	public:
		UIComponent();

		virtual ~UIComponent() override;


		void Init() override;

		void Start() override;


		/** @brief The canvas this component draws into, or null if it is not under one. */
		ComponentHandle<Canvas> GetCanvas() const;

		/** @brief This component's element, for other UI components that need to reference it. */
		Core::UIElement GetElement() const;

		void AddClass(std::string_view className);

		void RemoveClass(std::string_view className);

		/** @brief Adds or removes a style class. Safe to call before the component is initialised. */
		void SetClass(std::string_view className, bool enabled);

		bool HasClass(std::string_view className) const;

		bool HasState(Data::UIPseudoClass state) const;

		/**
		 * @brief Sets a style property directly, for anything the typed component API does not cover.
		 *
		 * This exposes the styling language, not the UI library: property names are engine enums and
		 * values are style syntax, which is the same deal a stylesheet file gets.
		 */
		void SetStyleProperty(Data::UIStyleProperty property, const std::string& value);

		/** @brief The element's resolved rectangle in canvas space. Empty before the first layout. */
		Core::RectF GetScreenRect() const;
	};
}