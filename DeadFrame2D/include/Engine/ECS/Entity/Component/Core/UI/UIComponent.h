#pragma once
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
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


namespace DF2D::Core
{
	class IUIBackend;
	class UIManager;
}


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
		Core::UIManager* uiManager = nullptr;

		ComponentHandle<Canvas> parentCanvas;

		Data::UIElementID element = 0;

		ComponentHandle<UIComponent> selfUIHandle;


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


		Core::IUIBackend* Backend() const;

		void SetStyle(Data::UIStyleProperty property, const std::string& value);

		void SetStyle(Data::UIStyleProperty property, float pixels);

		void SetStyle(Data::UIStyleProperty property, const Core::Color& color);

		void ClearStyle(Data::UIStyleProperty property);


	public:
		UIComponent();

		virtual ~UIComponent() override;


		void Init() override;


		/** @brief The canvas this component draws into, or null if it is not under one. */
		ComponentHandle<Canvas> GetCanvas() const;

		/** @brief This component's element, for other UI components that need to reference it. */
		Data::UIElementID GetElement() const;

		void AddClass(std::string_view className);

		void RemoveClass(std::string_view className);

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
