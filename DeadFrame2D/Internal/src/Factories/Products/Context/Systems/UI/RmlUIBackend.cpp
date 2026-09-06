#include "Converters/UI/RmlConversions.h"
#include "Converters/UI/RmlInputConversions.h"
#include "Factories/Products/Context/Systems/UI/RmlEventListener.h"
#include "Factories/Products/Context/Systems/UI/RmlFileInterface.h"
#include "Factories/Products/Context/Systems/UI/RmlRenderInterface.h"
#include "Factories/Products/Context/Systems/UI/RmlSystemInterface.h"
#include "Factories/Products/Context/Systems/UI/RmlUIBackend.h"
#include <iostream>
#include <RmlUi/Core.h>


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Internal::RmlConversions;
	using namespace DF2D::Internal::RmlInputConversions;


	namespace
	{
		/**
		 * @brief The document every context starts from.
		 *
		 * Deliberately empty: markup is an implementation detail here, and the component layer builds
		 * the whole tree through CreateElement. The body fills its context so percentage-based
		 * anchoring resolves against the full surface.
		 */
		constexpr const char* EmptyDocument =
			"<rml><head><style>"
			"body { width: 100%; height: 100%; margin: 0; }"
			"</style></head><body></body></rml>";


		/** @brief Events every element reports, so a component can subscribe after the fact. */
		constexpr Rml::EventId ForwardedEvents[] =
		{
			Rml::EventId::Click,
			Rml::EventId::Mouseover,
			Rml::EventId::Mouseout,
			Rml::EventId::Mousedown,
			Rml::EventId::Mouseup,
			Rml::EventId::Focus,
			Rml::EventId::Blur,
			Rml::EventId::Change,
			Rml::EventId::Submit
		};
	}


	RmlUIBackend::RmlUIBackend(TextureManager* textureManager, IUIPlatform* platform)
	{
		systemInterface = std::make_unique<RmlSystemInterface>(platform);
		fileInterface = std::make_unique<RmlFileInterface>();
		renderInterface = std::make_unique<RmlRenderInterface>(textureManager);

		// Order matters: the interfaces must be installed before Initialise, and must outlive Shutdown.
		Rml::SetSystemInterface(systemInterface.get());
		Rml::SetFileInterface(fileInterface.get());
		Rml::SetRenderInterface(renderInterface.get());

		initialised = Rml::Initialise();

		if (!initialised)
		{
			std::cerr << "Failed to initialise the UI backend." << std::endl;

			return;
		}

		std::cout << "[Info] UI backend successfully initialized (RmlUi " << Rml::GetVersion() << ")." << std::endl;
	}

	RmlUIBackend::~RmlUIBackend()
	{
		// Elements first, then contexts, then the library: an element outliving its context would be
		// dereferencing freed memory, and Shutdown frees everything the contexts still hold.
		elements.clear();
		contexts.clear();

		if (initialised)
		{
			Rml::Shutdown();

			std::cout << "[Info] UI backend successfully shut down." << std::endl;
		}

		Rml::SetSystemInterface(nullptr);
		Rml::SetFileInterface(nullptr);
		Rml::SetRenderInterface(nullptr);
	}


	Rml::Element* RmlUIBackend::FindElement(UIElementID element) const
	{
		auto it = elements.find(element);

		return it != elements.end() ? it->second.element : nullptr;
	}

	const RmlUIBackend::ContextEntry* RmlUIBackend::FindContext(UIContextID context) const
	{
		auto it = contexts.find(context);

		return it != contexts.end() ? &it->second : nullptr;
	}

	UIElementID RmlUIBackend::RegisterElement(Rml::Element* element, UIContextID context, Rml::ElementPtr owned)
	{
		if (element == nullptr)
			return 0;

		auto id = nextElementID++;

		auto& entry = elements[id];
		entry.element = element;
		entry.context = context;
		entry.detached = std::move(owned);

		entry.listener = std::make_unique<RmlEventListener>(eventSink, id);

		for (auto eventId : ForwardedEvents)
		{
			element->AddEventListener(eventId, entry.listener.get());
		}

		return id;
	}


	void RmlUIBackend::Advance(float deltaTime)
	{
		systemInterface->Advance(deltaTime);
	}

	void RmlUIBackend::SetEventSink(IUIEventSink* sink)
	{
		eventSink = sink;
	}


	UIContextID RmlUIBackend::CreateContext(Vector2I size)
	{
		if (!initialised)
			return 0;

		auto id = nextContextID++;

		auto name = "df2d_canvas_" + std::to_string(id);

		auto* context = Rml::CreateContext(name, Rml::Vector2i(size.x, size.y));

		if (context == nullptr)
		{
			// TODO: should I throw? returning 0 means something bad happend (right?)
			std::cerr << "Failed to create UI context " << name << "." << std::endl;

			return 0;
		}

		auto* document = context->LoadDocumentFromMemory(EmptyDocument);

		if (document == nullptr)
		{
			std::cerr << "Failed to create the root document for UI context " << name << "." << std::endl;

			Rml::RemoveContext(name);

			return 0;
		}

		document->Show();

		auto& entry = contexts[id];
		entry.context = context;
		entry.document = document;
		entry.rootElement = RegisterElement(document, id, nullptr);

		return id;
	}

	void RmlUIBackend::DestroyContext(UIContextID context)
	{
		auto it = contexts.find(context);

		if (it == contexts.end())
			return;

		// Every element in this context dies with it, so drop their handles before the memory goes.
		for (auto elementIt = elements.begin(); elementIt != elements.end(); )
		{
			elementIt = elementIt->second.context == context
				? elements.erase(elementIt)
				: std::next(elementIt);
		}

		if (it->second.context != nullptr)
		{
			Rml::RemoveContext(it->second.context->GetName());
		}

		contexts.erase(it);
	}

	void RmlUIBackend::SetContextSize(UIContextID context, Vector2I size)
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return;

		entry->context->SetDimensions(Rml::Vector2i(size.x, size.y));
	}

	void RmlUIBackend::UpdateContext(UIContextID context)
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return;

		entry->context->Update();
	}

	GeometryDrawList RmlUIBackend::RenderContext(UIContextID context)
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return {};

		// Render drives the interface callbacks, which record rather than draw; the resulting command
		// buffer is what the engine pipeline replays later in the frame.
		renderInterface->BeginRecording();

		entry->context->Render();

		return renderInterface->TakeRecording();
	}

	bool RmlUIBackend::LoadStyleSheet(UIContextID context, const std::string& path)
	{
		auto it = contexts.find(context);

		if (it == contexts.end() || it->second.document == nullptr)
			return false;

		auto sheet = Rml::Factory::InstanceStyleSheetFile(path);

		if (sheet == nullptr)
		{
			std::cerr << "Failed to load UI stylesheet " << path << "." << std::endl;

			return false;
		}

		// Sheets accumulate rather than replace, so loading a game theme keeps the engine defaults
		// underneath it and later sheets win on equal specificity.
		const auto* existing = it->second.document->GetStyleSheetContainer();

		it->second.document->SetStyleSheetContainer(
			existing != nullptr
				? existing->CombineStyleSheetContainer(*sheet)
				: sheet);

		it->second.styleSheets.push_back(path);

		return true;
	}

	UIElementID RmlUIBackend::GetRootElement(UIContextID context) const
	{
		const auto* entry = FindContext(context);

		return entry != nullptr ? entry->rootElement : 0;
	}


	UIElementID RmlUIBackend::CreateElement(UIContextID context, UIElementType type)
	{
		auto it = contexts.find(context);

		if (it == contexts.end() || it->second.document == nullptr)
			return 0;

		auto element = it->second.document->CreateElement(ToElementTag(type));

		if (element == nullptr)
			return 0;

		auto* raw = element.get();

		return RegisterElement(raw, context, std::move(element));
	}

	void RmlUIBackend::DestroyElement(UIElementID element)
	{
		auto it = elements.find(element);

		if (it == elements.end())
			return;

		auto* raw = it->second.element;

		// A parented element is owned by its parent, so ask the parent to give it back; the returned
		// pointer then falls out of scope and frees it. A still-detached one is freed by its own entry.
		if (raw != nullptr && it->second.detached == nullptr)
		{
			auto* parent = raw->GetParentNode();

			if (parent != nullptr)
			{
				parent->RemoveChild(raw);
			}
		}

		elements.erase(it);
	}

	void RmlUIBackend::SetElementParent(UIElementID element, UIElementID parent, int siblingIndex)
	{
		auto it = elements.find(element);

		if (it == elements.end() || it->second.element == nullptr)
			return;

		auto* parentElement = FindElement(parent);

		if (parentElement == nullptr)
			return;

		auto* raw = it->second.element;

		auto* adjacent = siblingIndex >= 0 && siblingIndex < parentElement->GetNumChildren()
			? parentElement->GetChild(siblingIndex)
			: nullptr;

		if (it->second.detached != nullptr)
		{
			// First attachment: hand ownership to the parent.
			auto owned = std::move(it->second.detached);

			if (adjacent != nullptr)
			{
				parentElement->InsertBefore(std::move(owned), adjacent);
			}
			else
			{
				parentElement->AppendChild(std::move(owned));
			}

			return;
		}

		auto* currentParent = raw->GetParentNode();

		if (currentParent == nullptr)
			return;

		auto owned = currentParent->RemoveChild(raw);

		if (owned == nullptr)
			return;

		if (adjacent != nullptr && adjacent != raw)
		{
			parentElement->InsertBefore(std::move(owned), adjacent);
		}
		else
		{
			parentElement->AppendChild(std::move(owned));
		}
	}

	void RmlUIBackend::SetElementProperty(UIElementID element, UIStyleProperty property, const std::string& value)
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return;

		raw->SetProperty(ToPropertyName(property), value);
	}

	void RmlUIBackend::ClearElementProperty(UIElementID element, UIStyleProperty property)
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return;

		raw->RemoveProperty(ToPropertyName(property));
	}

	void RmlUIBackend::SetElementAttribute(UIElementID element, UIAttribute attribute, const std::string& value)
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return;

		raw->SetAttribute(ToAttributeName(attribute), value);
	}

	void RmlUIBackend::SetElementText(UIElementID element, const std::string& text)
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return;

		// Encoded, not raw: game text is data and must never be able to inject markup into the tree.
		raw->SetInnerRML(Rml::StringUtilities::EncodeRml(text));
	}

	void RmlUIBackend::SetElementClass(UIElementID element, const std::string& className, bool enabled)
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return;

		raw->SetClass(className, enabled);
	}

	bool RmlUIBackend::HasElementClass(UIElementID element, const std::string& className) const
	{
		auto* raw = FindElement(element);

		return raw != nullptr && raw->IsClassSet(className);
	}

	bool RmlUIBackend::HasPseudoClass(UIElementID element, UIPseudoClass pseudoClass) const
	{
		auto* raw = FindElement(element);

		return raw != nullptr && raw->IsPseudoClassSet(ToPseudoClassName(pseudoClass));
	}

	void RmlUIBackend::SetElementVisible(UIElementID element, bool visible)
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return;

		// Removed from layout rather than merely invisible: an inactive UI object should stop taking
		// part in layout entirely, which is what deactivating its GameObject implies.
		raw->SetProperty("display", visible ? "block" : "none");
	}

	void RmlUIBackend::FocusElement(UIElementID element)
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return;

		raw->Focus();
	}

	RectF RmlUIBackend::GetElementRect(UIElementID element) const
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return {};

		auto offset = raw->GetAbsoluteOffset(Rml::BoxArea::Border);
		auto size = raw->GetBox().GetSize(Rml::BoxArea::Border);

		return RectF{ .x = offset.x, .y = offset.y, .w = size.x, .h = size.y };
	}

	Vector2F RmlUIBackend::GetElementContentSize(UIElementID element) const
	{
		auto* raw = FindElement(element);

		if (raw == nullptr)
			return Vector2F::Zero;

		auto size = raw->GetBox().GetSize(Rml::BoxArea::Content);

		return Vector2F(size.x, size.y);
	}


	void RmlUIBackend::ProcessMouseMove(UIContextID context, Vector2F position, KeyModifiers modifiers)
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return;

		entry->context->ProcessMouseMove(
			static_cast<int>(position.x),
			static_cast<int>(position.y),
			ToKeyModifierState(modifiers));
	}

	void RmlUIBackend::ProcessMouseButton(UIContextID context, Models::MouseButtonCode button, bool pressed, KeyModifiers modifiers)
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return;

		auto modifierState = ToKeyModifierState(modifiers);
		auto buttonIndex = ToButtonIndex(button);

		if (pressed)
		{
			entry->context->ProcessMouseButtonDown(buttonIndex, modifierState);
		}
		else
		{
			entry->context->ProcessMouseButtonUp(buttonIndex, modifierState);
		}
	}

	void RmlUIBackend::ProcessMouseWheel(UIContextID context, Vector2F delta, KeyModifiers modifiers)
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return;

		// Wheel deltas point right and down here, matching the library's own convention.
		entry->context->ProcessMouseWheel(
			Rml::Vector2f(delta.x, -delta.y),
			ToKeyModifierState(modifiers));
	}

	void RmlUIBackend::ProcessKey(UIContextID context, Models::KeyboardKeyCode key, bool pressed, KeyModifiers modifiers)
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return;

		auto identifier = ToKeyIdentifier(key);

		if (identifier == Rml::Input::KI_UNKNOWN)
			return;

		auto modifierState = ToKeyModifierState(modifiers);

		if (pressed)
		{
			entry->context->ProcessKeyDown(identifier, modifierState);
		}
		else
		{
			entry->context->ProcessKeyUp(identifier, modifierState);
		}
	}

	void RmlUIBackend::ProcessTextInput(UIContextID context, const std::string& text)
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return;

		entry->context->ProcessTextInput(Rml::String(text));
	}

	bool RmlUIBackend::HasKeyboardFocus(UIContextID context) const
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return false;

		auto* focused = entry->context->GetFocusElement();

		// The document itself always holds focus when nothing else does, so it does not count.
		return focused != nullptr && focused != static_cast<Rml::Element*>(entry->document);
	}

	bool RmlUIBackend::IsPointerOverElement(UIContextID context) const
	{
		const auto* entry = FindContext(context);

		if (entry == nullptr)
			return false;

		auto* hovered = entry->context->GetHoverElement();

		return hovered != nullptr && hovered != static_cast<Rml::Element*>(entry->document);
	}

	bool RmlUIBackend::LoadFontFace(const std::string& path, const std::string& family, bool fallbackFace)
	{
		if (!initialised)
			return false;

		if (family.empty())
			return Rml::LoadFontFace(path, fallbackFace);

		return Rml::LoadFontFace(path, family, Rml::Style::FontStyle::Normal, Rml::Style::FontWeight::Auto, fallbackFace);
	}
}