#include "Constants/Paths/ResourcePaths.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include <algorithm>
#include <iostream>


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
			if (owner == nullptr)
				continue;

			owner->HandleUIEvent(eventType, payload);
		}
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

	IUIBackend& UIManager::Backend()
	{
		return *backend;
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

	UIElementID UIManager::AcquireElement(UIContextID context, const void* owningObject, UIElementType type)
	{
		if (context == 0 || owningObject == nullptr)
			return 0;

		auto it = objectElements.find(owningObject);

		if (it != objectElements.end())
		{
			if (it->second.type != type)
			{
				std::cerr << "A UI object already has an element of a different kind; keeping the existing one."
					<< " Split the components across two objects if both kinds are needed." << std::endl;
			}

			it->second.referenceCount++;

			return it->second.element;
		}

		auto element = backend->CreateElement(context, type);

		if (element == 0)
			return 0;

		objectElements[owningObject] = SharedElement
		{
			.element = element,
			.type = type,
			.referenceCount = 1
		};

		return element;
	}

	void UIManager::ReleaseElement(const void* owningObject)
	{
		auto it = objectElements.find(owningObject);

		if (it == objectElements.end())
			return;

		if (--it->second.referenceCount > 0)
			return;

		backend->DestroyElement(it->second.element);

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