#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/UI/Abstractions/IUIBackend.h"
#include "Core/Context/Systems/UI/Abstractions/IUIEventSink.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/GeometryDrawList.h"
#include "Data/Systems/UI/UIContextID.h"
#include "Data/Systems/UI/UIElementID.h"
#include "Data/Systems/UI/UIElementType.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace DF2D::Engine
{
	class UIComponent;

	template<typename T>
	class ComponentHandle;
}


namespace DF2D::Core
{
	/**
	 * @brief Owns the UI backend and routes between it and the component layer.
	 *
	 * Pure orchestration: it caches which fonts have been loaded, tracks which component owns which
	 * element so backend events can be delivered, and drives the per-frame update and render points.
	 * Every call that touches the underlying UI library goes through IUIBackend.
	 */
	class DF2D_API UIManager : public ICoreSystem, public IUIEventSink
	{
	private:
		struct SharedElement
		{
			Data::UIElementID element = 0;

			Data::UIElementType type = Data::UIElementType::PANEL;

			int referenceCount = 0;
		};


		std::unique_ptr<IUIBackend> backend;

		std::unordered_set<std::string> loadedFonts;

		/**
		 * @brief Which components own which element, for delivering backend events.
		 *
		 * A list per element, not a single owner: one element is shared by every UI component sitting on
		 * the same GameObject, so a Text and an Image on one object both need to hear about it.
		 */
		std::unordered_map<Data::UIElementID, std::vector<Engine::ComponentHandle<Engine::UIComponent>>> elementOwners;

		/** @brief Live contexts, so every canvas can be updated without the scene walking them. */
		std::unordered_set<Data::UIContextID> activeContexts;

		/** @brief One element per UI GameObject, shared by every UI component sitting on it. */
		std::unordered_map<const void*, SharedElement> objectElements;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		void OnUIEvent(Data::UIElementID element, Data::UIEventType eventType, const Data::UIEventPayload& payload) override;


	public:
		UIManager(std::unique_ptr<IUIBackend> backend);

		~UIManager() override;

		UIManager(const UIManager&) = delete;

		UIManager(UIManager&&) = delete;

		UIManager& operator=(const UIManager&) = delete;

		UIManager& operator=(UIManager&&) = delete;


		/**
		 * @brief Registers a font file so styling can select it by family name.
		 *
		 * @param family: Family name to register the face under; empty keeps the name inside the file.
		 * @param fallbackFace: Whether the face may supply glyphs missing from other faces.
		 */
		bool LoadFont(std::string_view path, std::string_view family = "", bool fallbackFace = false);


		/** @brief Direct access to the backend for the UI components that drive it. */
		IUIBackend& Backend();

		Data::UIContextID CreateContext(Vector2I size);

		void DestroyContext(Data::UIContextID context);

		/** @brief Renders one context into a draw list, ready to be submitted as a render task. */
		Data::GeometryDrawList RenderContext(Data::UIContextID context);

		/**
		 * @brief Returns the element backing a UI GameObject, creating it on first request.
		 *
		 * The element belongs to the GameObject, not to the component that asked: a single object
		 * carrying a RectTransform, an Image and a Text is one styled element, not three siblings.
		 * Its type is fixed by whichever UI component asks first.
		 */
		Data::UIElementID AcquireElement(Data::UIContextID context, const void* owningObject, Data::UIElementType type);

		/** @brief Drops one reference to a GameObject element, destroying it when the last one goes. */
		void ReleaseElement(const void* owningObject);

		/** @brief Adds a component to the list that receives events for an element. */
		void RegisterElementOwner(Data::UIElementID element, const Engine::ComponentHandle<Engine::UIComponent>& owner);

		/** @brief Removes one component from an element's owner list, dropping the list when it empties. */
		void UnregisterElementOwner(Data::UIElementID element, const Engine::ComponentHandleBase& owner);
	};
}