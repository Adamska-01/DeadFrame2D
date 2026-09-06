#include "Constants/Paths/ResourcePaths.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <variant>


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Engine;


	namespace
	{
		// Must match the font-family in the engine default stylesheet.
		constexpr std::string_view DefaultFontFamily = "DeadFrame";
	}


	UIManager::UIManager(std::unique_ptr<IUIBackend> backend)
		: backend(std::move(backend))
	{
		this->backend->SetEventSink(this);

		// Registered under an engine-owned family name rather than whatever is baked into the file, so
		// the default stylesheet keeps resolving even if the shipped font is swapped out.
		LoadFont(Constants::Paths::Files::CONSOLAS_FONT, DefaultFontFamily, true);
	}

	UIManager::~UIManager()
	{
		elementOwners.clear();
		objectElements.clear();
		activeContexts.clear();

		backend.reset();
	}


	void UIManager::BeginFrame()
	{

	}

	void UIManager::PreUpdate(float deltaTime)
	{

	}

	void UIManager::EndUpdate(float deltaTime)
	{
		// Layout resolves here, after components have finished writing style properties in Update and
		// before LateUpdate, which is where they read resolved rectangles back.
		for (auto context : activeContexts)
		{
			backend->UpdateContext(context);
		}
	}

	void UIManager::EndDraw()
	{

	}


	void UIManager::OnUIEvent(UIElementID element, UIEventType eventType, const UIEventPayload& payload)
	{
		auto it = elementOwners.find(element);

		if (it == elementOwners.end())
			return;

		// Copied, because a handler is free to destroy components or reparent objects, either of which
		// would mutate the list being walked.
		auto owners = it->second;

		for (auto& owner : owners)
		{
			if (owner == nullptr || !owner->IsActive())
				continue;

			owner->HandleUIEvent(eventType, payload);
		}
	}


	bool UIManager::CapturesPointer() const
	{
		for (auto context : activeContexts)
		{
			if (backend->IsPointerOverElement(context))
				return true;
		}

		return false;
	}

	bool UIManager::CapturesKeyboard() const
	{
		for (auto context : activeContexts)
		{
			if (backend->HasKeyboardFocus(context))
				return true;
		}

		return false;
	}

	IUIBackend& UIManager::Backend()
	{
		return *backend;
	}

	void UIManager::OnSystemEvent(const SystemEvent& systemEvent)
	{
		for (auto context : activeContexts)
		{
			std::visit(
				[&](const auto& event)
				{
					using T = std::decay_t<decltype(event)>;

					if constexpr (std::is_same_v<T, MouseMoveEvent>)
					{
						backend->ProcessMouseMove(context, event.position, event.modifiers);
					}
					else if constexpr (std::is_same_v<T, MouseButtonEvent>)
					{
						backend->ProcessMouseButton(context, event.button, event.pressed, event.modifiers);
					}
					else if constexpr (std::is_same_v<T, MouseWheelEvent>)
					{
						backend->ProcessMouseWheel(context, event.delta, event.modifiers);
					}
					else if constexpr (std::is_same_v<T, KeyEvent>)
					{
						backend->ProcessKey(context, event.key, event.pressed, event.modifiers);
					}
					else if constexpr (std::is_same_v<T, TextInputEvent>)
					{
						backend->ProcessTextInput(context, event.text);
					}
				},
				systemEvent);
		}
	}

	void UIManager::SetContextSize(UIContextID context, Vector2I size)
	{
		if (context == 0)
			return;

		backend->SetContextSize(context, size);
	}

	bool UIManager::LoadStyleSheet(UIContextID context, const std::string& path)
	{
		if (context == 0)
			return false;

		return backend->LoadStyleSheet(context, path);
	}

	UIElementID UIManager::GetRootElement(UIContextID context) const
	{
		if (context == 0)
			return 0;

		return backend->GetRootElement(context);
	}

	bool UIManager::LoadFont(std::string_view path, std::string_view family, bool fallbackFace)
	{
		auto pathString = std::string(path);

		if (loadedFonts.contains(pathString))
			return true;

		if (!backend->LoadFontFace(pathString, std::string(family), fallbackFace))
			return false;

		loadedFonts.insert(std::move(pathString));

		return true;
	}

	UIContextID UIManager::CreateContext(Vector2I size)
	{
		auto context = backend->CreateContext(size);

		if (context == 0)
			return 0;

		activeContexts.insert(context);

		return context;
	}

	void UIManager::DestroyContext(UIContextID context)
	{
		if (context == 0)
			return;

		activeContexts.erase(context);

		backend->DestroyContext(context);
	}

	GeometryDrawList UIManager::RenderContext(UIContextID context)
	{
		if (context == 0)
			return {};

		return backend->RenderContext(context);
	}

	void UIManager::DeclareElementType(UIContextID context, const void* owningObject, UIElementType type)
	{
		if (context == 0 || owningObject == nullptr)
			return;

		auto& shared = objectElements[owningObject];

		// PANEL is the absence of an opinion (RectTransform), so it never overrides one that has already been made.
		if (type == UIElementType::PANEL)
			return;

		if (shared.type != UIElementType::PANEL && shared.type != type)
		{
			std::cerr << "A UI object needs two different kinds of element at once; keeping the first."
				<< " Split the components across two objects." << std::endl;

			return;
		}

		shared.type = type;
	}

	UIElementID UIManager::AcquireElement(UIContextID context, const void* owningObject)
	{
		if (context == 0 || owningObject == nullptr)
			return 0;

		auto& shared = objectElements[owningObject];

		shared.referenceCount++;

		if (shared.element != 0)
			return shared.element;

		// Built from whatever was declared during Init, which is why creation waits until Start: by
		// now every component on this object has had its say about what kind of element it needs.
		shared.element = backend->CreateElement(context, shared.type);

		return shared.element;
	}

	void UIManager::ReleaseElement(const void* owningObject)
	{
		auto it = objectElements.find(owningObject);

		if (it == objectElements.end())
			return;

		if (--it->second.referenceCount > 0)
			return;

		if (it->second.element != 0)
		{
			backend->DestroyElement(it->second.element);
		}

		objectElements.erase(it);
	}

	void UIManager::RegisterElementOwner(UIElementID element, const ComponentHandle<UIComponent>& owner)
	{
		if (element == 0 || owner == nullptr)
			return;

		elementOwners[element].push_back(owner);
	}

	void UIManager::UnregisterElementOwner(UIElementID element, const ComponentHandleBase& owner)
	{
		auto it = elementOwners.find(element);

		if (it == elementOwners.end())
			return;

		auto& owners = it->second;

		owners.erase(
			std::remove_if(
				owners.begin(),
				owners.end(),
				[&owner](const ComponentHandleBase& entry)
				{
					// Expired entries go at the same time
					return entry == owner || !entry.IsValid();
				}),
			owners.end());

		if (owners.empty())
		{
			elementOwners.erase(it);
		}
	}
}