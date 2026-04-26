#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Engine/ECS/Component/Rendering/Camera.h"
#include "Engine/ECS/Component/Rendering/Camera/CameraFollow.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Entity/GameObject.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
#include "Utilities/Helpers/Events/EventHelpers.h"
#include <algorithm>
#include <limits>


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Utilities;


	CameraFollow::CameraFollow(ComponentHandle<Camera> camera, ObjectHandle<GameObject> target)
		: camera(camera),
		target(target),
		offset(Vector2F::Zero),
		followSpeed(10.0f)
	{
		worldBounds = SDL_FRect
		{ 
			-std::numeric_limits<float>::infinity(),
			-std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity()
		};

		resolutionTarget = Renderer::GetResolutionTarget();

		EventDispatcher::RegisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), this, &CameraFollow::RenderTargetSizeChangedEventHandler);
	}

	CameraFollow::~CameraFollow()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), this);
	}

	void CameraFollow::RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto renderTargetSizeChangeEvent = DispatchableEvent::SafeCast<RenderTargetSizeChangedEvent>(dispatchableEvent);

		if (renderTargetSizeChangeEvent == nullptr)
			return;

		resolutionTarget = renderTargetSizeChangeEvent->renderTargetSize;
	}

	void CameraFollow::Update(float deltaTime)
	{
		if (!target || !camera)
			return;

		auto cameraGO = camera->GetGameObject();

		if (!cameraGO)
			return;

		auto cameraTransform = cameraGO->GetTransform();
		auto targetTransform = target->GetTransform();

		const auto targetPos = targetTransform->GetWorldPosition();
		const auto zoom = std::max(camera->GetZoom(), 0.01f);
		const auto res = Renderer::GetResolutionTarget();

		const auto viewSize = Vector2F
		{
			static_cast<float>(res.x) / zoom,
			static_cast<float>(res.y) / zoom
		};

		const auto halfView = viewSize * 0.5f;
		auto desiredCenter = targetPos + offset;

		// Clamp inside world bounds
		const auto xMin = worldBounds.x + halfView.x;
		const auto xMax = worldBounds.x + worldBounds.w - halfView.x;
		const auto yMin = worldBounds.y + halfView.y;
		const auto yMax = worldBounds.y + worldBounds.h - halfView.y;

		desiredCenter.x = (worldBounds.w < viewSize.x)
			? (worldBounds.x + worldBounds.w * 0.5f)
			: std::clamp(desiredCenter.x, xMin, xMax);

		desiredCenter.y = (worldBounds.h < viewSize.y)
			? (worldBounds.y + worldBounds.h * 0.5f)
			: std::clamp(desiredCenter.y, yMin, yMax);

		const auto currentPos = cameraTransform->GetWorldPosition();
		const auto t = std::clamp(followSpeed * deltaTime, 0.0f, 1.0f);
		const auto newPos = Vector2F::Lerp(currentPos, desiredCenter, t);

		cameraTransform->SetWorldPosition(newPos);
	}

	void CameraFollow::SetTarget(ObjectHandle<GameObject> newTarget)
	{
		target = newTarget;
	}

	void CameraFollow::SetBounds(const SDL_FRect& bounds)
	{
		worldBounds = bounds;
	}

	void CameraFollow::SetOffset(Vector2F newOffset)
	{
		offset = newOffset;
	}

	void CameraFollow::SetFollowSpeed(float speed)
	{
		followSpeed = speed;
	}
}