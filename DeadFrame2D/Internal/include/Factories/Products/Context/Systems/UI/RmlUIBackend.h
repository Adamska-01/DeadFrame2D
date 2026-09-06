#pragma once
#include "Core/Context/Systems/UI/Abstractions/IUIBackend.h"
#include "DF2D_API.h"
#include <memory>
#include <RmlUi/Core/Types.h>
#include <unordered_map>
#include <vector>


namespace Rml
{
	class ElementDocument;
}


namespace DF2D::Core
{
	class TextureManager;
}


namespace DF2D::Internal
{
	class RmlEventListener;
	class RmlFileInterface;
	class RmlRenderInterface;
	class RmlSystemInterface;


	/**
	 * @brief The RmlUi implementation of the engine's UI backend.
	 *
	 * This is the only class in the engine that knows RmlUi exists. It owns the library's global
	 * lifetime, so exactly one may exist at a time, and it must outlive every context it hands out.
	 */
	class DF2D_API RmlUIBackend : public Core::IUIBackend
	{
	private:
		struct ContextEntry
		{
			Rml::Context* context = nullptr;

			Rml::ElementDocument* document = nullptr;

			Data::UIElementID rootElement = 0;

			/** @brief Stylesheets applied so far, kept so a later one can be combined onto them. */
			std::vector<std::string> styleSheets;
		};

		struct ElementEntry
		{
			Rml::Element* element = nullptr;

			Data::UIContextID context = 0;

			/** @brief Owns the element while it is detached; null once a parent has taken it. */
			Rml::ElementPtr detached;

			std::unique_ptr<RmlEventListener> listener;
		};


		std::unique_ptr<RmlSystemInterface> systemInterface;

		std::unique_ptr<RmlFileInterface> fileInterface;

		std::unique_ptr<RmlRenderInterface> renderInterface;

		Core::IUIEventSink* eventSink = nullptr;

		std::unordered_map<Data::UIContextID, ContextEntry> contexts;

		Data::UIContextID nextContextID = 1;

		std::unordered_map<Data::UIElementID, ElementEntry> elements;

		Data::UIElementID nextElementID = 1;

		bool initialised = false;


		Rml::Element* FindElement(Data::UIElementID element) const;

		const ContextEntry* FindContext(Data::UIContextID context) const;

		/** @brief Wraps a live Rml element in an engine handle and hooks up its event listener. */
		Data::UIElementID RegisterElement(Rml::Element* element, Data::UIContextID context, Rml::ElementPtr owned);


	public:
		RmlUIBackend(Core::TextureManager* textureManager, Core::IUIPlatform* platform);

		~RmlUIBackend() override;

		RmlUIBackend(const RmlUIBackend&) = delete;

		RmlUIBackend(RmlUIBackend&&) = delete;

		RmlUIBackend& operator=(const RmlUIBackend&) = delete;

		RmlUIBackend& operator=(RmlUIBackend&&) = delete;


		/** @brief Advances the UI clock. Fed unscaled delta so a paused game still animates its UI. */
		void Advance(float deltaTime);


		void SetEventSink(Core::IUIEventSink* sink) override;

		Data::UIContextID CreateContext(Core::Vector2I size) override;

		void DestroyContext(Data::UIContextID context) override;

		void SetContextSize(Data::UIContextID context, Core::Vector2I size) override;

		void UpdateContext(Data::UIContextID context) override;

		Data::GeometryDrawList RenderContext(Data::UIContextID context) override;

		bool LoadStyleSheet(Data::UIContextID context, const std::string& path) override;

		Data::UIElementID GetRootElement(Data::UIContextID context) const override;

		Data::UIElementID CreateElement(Data::UIContextID context, Data::UIElementType type) override;

		void DestroyElement(Data::UIElementID element) override;

		void SetElementParent(Data::UIElementID element, Data::UIElementID parent, int siblingIndex) override;

		void SetElementProperty(Data::UIElementID element, Data::UIStyleProperty property, const std::string& value) override;

		void ClearElementProperty(Data::UIElementID element, Data::UIStyleProperty property) override;

		void SetElementAttribute(Data::UIElementID element, Data::UIAttribute attribute, const std::string& value) override;

		void SetElementText(Data::UIElementID element, const std::string& text) override;

		void SetElementClass(Data::UIElementID element, const std::string& className, bool enabled) override;

		bool HasElementClass(Data::UIElementID element, const std::string& className) const override;

		bool HasPseudoClass(Data::UIElementID element, Data::UIPseudoClass pseudoClass) const override;

		void SetElementVisible(Data::UIElementID element, bool visible) override;

		void FocusElement(Data::UIElementID element) override;

		Core::RectF GetElementRect(Data::UIElementID element) const override;

		Core::Vector2F GetElementContentSize(Data::UIElementID element) const override;

		void ProcessMouseMove(Data::UIContextID context, Core::Vector2F position, Data::KeyModifiers modifiers) override;

		void ProcessMouseButton(Data::UIContextID context, Models::MouseButtonCode button, bool pressed, Data::KeyModifiers modifiers) override;

		void ProcessMouseWheel(Data::UIContextID context, Core::Vector2F delta, Data::KeyModifiers modifiers) override;

		void ProcessKey(Data::UIContextID context, Models::KeyboardKeyCode key, bool pressed, Data::KeyModifiers modifiers) override;

		void ProcessTextInput(Data::UIContextID context, const std::string& text) override;

		bool HasKeyboardFocus(Data::UIContextID context) const override;

		bool IsPointerOverElement(Data::UIContextID context) const override;

		bool LoadFontFace(const std::string& path, const std::string& family, bool fallbackFace) override;
	};
}