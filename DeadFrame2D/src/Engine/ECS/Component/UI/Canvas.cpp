#include "Constants/Paths/ResourcePaths.h"
#include "Constants/Rendering/DefaultSortOrders.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
#include "Utilities/Debugging/Guards.h"
#include "Utilities/Helpers/Events/EventHelpers.h"


namespace DF2D::Engine
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	Canvas::Canvas()
		: sortOrder(DefaultSortOrders::UI_RENDERER),
		renderMode(CanvasRenderMode::SCREEN_SPACE_OVERLAY)
	{
	}

	Canvas::~Canvas()
	{
		if (uiManager == nullptr || context == 0)
			return;

		uiManager->DestroyContext(context);
	}


	void Canvas::Init()
	{
		auto* renderer = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().renderer, NAME_OF(renderer));

		uiManager = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().uiManager, NAME_OF(uiManager));

		auto resolution = renderer->GetResolutionTarget();

		context = uiManager->CreateContext(resolution);

		// TODO: Don't really like calling base function after some code. See if it can be polished.
		// The context must exist before UIComponent::Init runs, because that is what resolves this
		// canvas and asks it for an element.
		UIComponent::Init();

		auto* eventDispatcher = GetGameObject()->ServiceContext().eventDispatcher;

		if (eventDispatcher != nullptr)
		{
			eventDispatcher->RegisterEventHandler<RenderTargetSizeChangedEvent>(
				GetHandle(),
				EventHelpers::BindFunction(this, &Canvas::RenderTargetSizeChangedEventHandler));
		}
	}

	void Canvas::OnElementCreated()
	{
		// Engine defaults first so a game stylesheet loaded later layers on top of them.
		LoadStyleSheet(std::string_view(Paths::Files::DEFAULT_UI_STYLESHEET));

		auto queued = std::move(pendingStyleSheets);

		pendingStyleSheets.clear();

		for (const auto& path : queued)
		{
			LoadStyleSheet(path);
		}
	}

	void Canvas::RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto resized = DispatchableEvent::SafeCast<RenderTargetSizeChangedEvent>(dispatchableEvent);

		if (resized == nullptr || uiManager == nullptr || context == 0)
			return;

		uiManager->Backend().SetContextSize(context, resized->renderTargetSize);
	}


	// TODO: The RMLUI lib seems to be rendering only the canvas (starts from the root, providing the geometry
	// for the whole context). This means that an optimization can be done in Scene::Draw (I guess also Scene::Update)
	// and just call the draw function on the GameObjects with a Canvas, and not the ones below. It would be nice
	// to have some kind of reflection like in Unity, where the Engine already knows if a component implements
	// functions like "Update", "LateUpdate" etc. (Those are special functions in Unity, not virtual, magic/lifecycle methods)
	void Canvas::Draw()
	{
		// TODO: Is context check necessary here? If the TODO in the context assignment is addressed
		// ("Should I throw here?" comment), then remove the check, since it being 0 wouldn't be possible.
		// Also, is the "IsActive()" check necessary? before calling draw, the Scene::Draw() already 
		// checks if the gameObject and component are active.
		if (uiManager == nullptr || context == 0 || !IsActive())
			return;

		auto drawList = uiManager->RenderContext(context);

		if (drawList.commands.empty())
			return;

		renderTask.renderPhase = renderMode == CanvasRenderMode::SCREEN_SPACE_CAMERA
			? RenderPhase::SCREEN_SPACE_CAMERA_UI
			: RenderPhase::SCREEN_SPACE_OVERLAY_UI;

		renderTask.sortOrder = sortOrder;
		renderTask.canvas = GetHandleAs<Canvas>();

		// The whole canvas travels as one task: its commands are already in the order they must be
		// drawn, and splitting them would let the pipeline's sort key shuffle them.
		renderTask.renderData = GeometryRenderData
		{
			.drawList = std::make_shared<const GeometryDrawList>(std::move(drawList))
		};

		RenderSystem::Submit(renderTask);
	}


	UIElementID Canvas::AcquireElementFor(const ObjectHandle<GameObject>& owner, UIElementType type)
	{
		if (uiManager == nullptr || context == 0)
			return 0;

		// The canvas object itself is the context root rather than a child element of it.
		if (owner == GetGameObject())
			return uiManager->Backend().GetRootElement(context);

		return uiManager->AcquireElement(context, owner.operator->(), type);
	}

	bool Canvas::LoadStyleSheet(std::string_view path)
	{
		// Scenes load their theme in Enter, which runs before any component is initialised and so
		// before the context exists. The request is queued and replayed once it does.
		if (uiManager == nullptr || context == 0)
		{
			pendingStyleSheets.push_back(std::string(path));

			return true;
		}

		return uiManager->Backend().LoadStyleSheet(context, std::string(path));
	}

	UIContextID Canvas::GetContext() const
	{
		return context;
	}

	CanvasRenderMode Canvas::GetRenderMode() const
	{
		return renderMode;
	}

	int Canvas::GetSortOrder() const
	{
		return sortOrder;
	}

	ComponentHandle<Camera> Canvas::GetRenderCamera()
	{
		return renderCamera;
	}

	void Canvas::SetSortOrder(int value)
	{
		sortOrder = value;
	}

	void Canvas::SetScreenSpaceOverlay()
	{
		renderMode = CanvasRenderMode::SCREEN_SPACE_OVERLAY;

		renderCamera = nullptr;
	}

	void Canvas::SetScreenSpaceCamera(ComponentHandle<Camera> targetCamera)
	{
		renderMode = targetCamera != nullptr
			? CanvasRenderMode::SCREEN_SPACE_CAMERA
			: CanvasRenderMode::SCREEN_SPACE_OVERLAY;

		renderCamera = targetCamera;
	}
}