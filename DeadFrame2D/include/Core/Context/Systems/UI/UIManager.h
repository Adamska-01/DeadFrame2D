#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Input/Abstractions/IInputCaptureState.h"
#include "Core/Context/Systems/UI/Abstractions/IUIBackend.h"
#include "Core/Context/Systems/UI/Abstractions/IUIEventSink.h"
#include "Core/Context/Systems/UI/UIContext.h"
#include "Core/Math/Vector2.h"
#include "Core/Services/Events/Abstractions/ISystemEventSink.h"
#include "Core/Services/Time/Abstractions/ITimeProvider.h"
#include "Data/Systems/Rendering/Pipeline/GeometryDrawList.h"
#include "Data/Systems/UI/UIContextID.h"
#include "Data/Systems/UI/UIElementID.h"
#include "Data/Systems/UI/UIElementType.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Utilities/Hashing/ObjectHandleHash.h"
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace DF2D::Engine
{
	class GameObject;
	class UIComponent;

	template<typename T>
	class ComponentHandle;
}


namespace DF2D::Core
{
	/**
	 * @brief Owns the UI backend and routes between it and the component layer.
	 */
	class DF2D_API UIManager : public ICoreSystem, public IUIEventSink, public ISystemEventSink, public IInputCaptureState
	{
		friend class UIContext;
		friend class UIElement;


	private:
		struct SharedElement
		{
			Data::UIElementID element = 0;

			Data::UIElementType type = Data::UIElementType::PANEL;

			int referenceCount = 0;
		};


		std::unique_ptr<IUIBackend> backend;

		/** @brief Drives the UI clock. Read unscaled, so a paused game still animates its UI. */
		ITimeProvider* timeProvider = nullptr;

		std::unordered_set<std::string> loadedFonts;

		/** @brief Which components own which element, for delivering backend events. */
		std::unordered_map<Data::UIElementID, std::vector<Engine::ComponentHandle<Engine::UIComponent>>> elementOwners;

		/** @brief Live contexts, so every canvas can be updated without the scene walking them. */
		std::unordered_set<Data::UIContextID> activeContexts;

		/** @brief One element per UI GameObject, shared by every UI component sitting on it. */
		std::unordered_map<Engine::ObjectHandle<Engine::GameObject>, SharedElement, Utilities::ObjectHandleHash> objectElements;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		void OnUIEvent(Data::UIElementID element, Data::UIEventType eventType, const Data::UIEventPayload& payload) override;


		void OnSystemEvent(const Data::SystemEvent& systemEvent) override;


		bool CapturesPointer() const override;

		bool CapturesKeyboard() const override;


		/** @brief Resizes the surface that percentage lengths resolve against. */
		void SetContextSize(Data::UIContextID context, Vector2I size);

		/** @brief Applies a stylesheet on top of the ones a surface has already loaded. */
		bool LoadStyleSheet(Data::UIContextID context, const std::string& path);

		/** @brief The root element of a surface, which its canvas component adopts. */
		Data::UIElementID GetRootElement(Data::UIContextID context) const;

		/** @brief Renders one surface into a draw list, ready to submit as a render task. */
		Data::GeometryDrawList RenderContext(Data::UIContextID context);

		/** @brief Sets an inline style property, which outranks every stylesheet rule. */
		void SetElementProperty(Data::UIElementID element, Data::UIStyleProperty property, const std::string& value);

		/** @brief Drops an inline property so the stylesheets decide the value again. */
		void ClearElementProperty(Data::UIElementID element, Data::UIStyleProperty property);

		/** @brief Sets a non-style attribute, such as an image source or an input's value. */
		void SetElementAttribute(Data::UIElementID element, Data::UIAttribute attribute, const std::string& value);

		/** @brief Drops an attribute, for the ones whose presence alone is the state. */
		void RemoveElementAttribute(Data::UIElementID element, Data::UIAttribute attribute);

		/** @brief Replaces the element's text content. */
		void SetElementText(Data::UIElementID element, const std::string& text);

		/** @brief Adds or removes a style class on the element. */
		void SetElementClass(Data::UIElementID element, const std::string& className, bool enabled);

		/** @brief How far the element's content is currently scrolled. */
		bool HasElementClass(Data::UIElementID element, const std::string& className) const;

		/** @brief Whether the element is in the given interaction state, such as hovered or pressed. */
		bool HasPseudoClass(Data::UIElementID element, Data::UIPseudoClass pseudoClass) const;

		/** @brief Shows or hides the element, taking it out of layout entirely while hidden. */
		void SetElementVisible(Data::UIElementID element, bool visible);

		/** @brief Moves an element under a new parent. A negative index appends. */
		void SetElementParent(Data::UIElementID element, Data::UIElementID parent, int siblingIndex);

		/** @brief The element's resolved border box, in context space. */
		Core::RectF GetElementRect(Data::UIElementID element) const;

		/** @brief The size the element's content wants, independent of the box it was given. */
		Vector2F GetElementContentSize(Data::UIElementID element) const;

		Vector2F GetElementScrollOffset(Data::UIElementID element) const;

		/** @brief Scrolls the element's content, clamped by the backend to what there is to scroll. */
		void SetElementScrollOffset(Data::UIElementID element, Vector2F offset);

		/** @brief The full size of the element's content, which is what it scrolls within. */
		Vector2F GetElementScrollSize(Data::UIElementID element) const;

		/** @brief Appends an option to a dropdown and returns its index, or -1 if it could not be added. */
		int AddDropdownOption(Data::UIElementID dropdown, const std::string& text, const std::string& value);

		/** @brief Removes every option from a dropdown. */
		void ClearDropdownOptions(Data::UIElementID dropdown);

		/** @brief Selects an option by index. An index outside the list clears the selection. */
		void SetDropdownSelection(Data::UIElementID dropdown, int index);

		/** @brief The selected option's index, or -1 when nothing is selected. */
		int GetDropdownSelection(Data::UIElementID dropdown) const;

		/** @brief Destroys a context and every element inside it, and stops tracking it as live. */
		void DestroyContext(Data::UIContextID context);

		/**
		 * @brief Records what kind of element a UI GameObject needs, without creating it yet.
		 *
		 * The most specific declaration wins; PANEL means no opinion.
		 */
		void DeclareElementType(Data::UIContextID context, const Engine::ObjectHandle<Engine::GameObject>& owningObject, Data::UIElementType type);

		/**
		 * @brief Returns the element backing a UI GameObject, creating it on first request.
		 *
		 * The element belongs to the GameObject, not to the component that asked: a single object
		 * carrying a RectTransform, an Image and a Text is one styled element, not three siblings.
		 */
		Data::UIElementID AcquireElement(Data::UIContextID context, const Engine::ObjectHandle<Engine::GameObject>& owningObject);

		/** @brief Drops one reference to a GameObject element, destroying it when the last one goes. */
		void ReleaseElement(const Engine::ObjectHandle<Engine::GameObject>& owningObject);

		/** @brief Adds a component to the list that receives events for an element. */
		void RegisterElementOwner(Data::UIElementID element, const Engine::ComponentHandle<Engine::UIComponent>& owner);

		/** @brief Removes one component from an element's owner list, dropping the list when it empties. */
		void UnregisterElementOwner(Data::UIElementID element, const Engine::ComponentHandleBase& owner);


	public:
		UIManager(std::unique_ptr<IUIBackend> backend, ITimeProvider* timeProvider);

		~UIManager() override;

		UIManager(const UIManager&) = delete;

		UIManager(UIManager&&) = delete;

		UIManager& operator=(const UIManager&) = delete;

		UIManager& operator=(UIManager&&) = delete;


		bool LoadFont(std::string_view path, std::string_view family = "", bool fallbackFace = false);

		UIContext CreateCanvasContext(Vector2I size);
	};
}