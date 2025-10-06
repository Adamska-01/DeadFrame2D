#include "Core/SubSystems/Systems/Renderer.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Components/Transform.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Renderer/RenderTargetSizeChangedEvent.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include "Utilities/Debugging/Guards.h"
#include "Utilities/Helpers/Events/EventHelpers.h"
#include <typeindex>


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Utilities;


	std::vector<Camera*> Camera::cameras = {};


	Camera::Camera()
		: transform(nullptr),
		// Fullscreen (Doesn't work currently)
		normalizedViewport({ 0.0f, 0.0f, 1.0f, 1.0f }),
		zoom(1.0f)
	{
		cameras.push_back(this);

		resolutionTarget = Renderer::GetResolutionTarget();

		EventDispatcher::RegisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), BindFunction(this, &Camera::RenderTargetSizeChangedEventHandler), reinterpret_cast<uintptr_t>(this));
	}

	Camera::~Camera()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), reinterpret_cast<uintptr_t>(this));

		std::erase(cameras, this);
	}

	void Camera::RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto renderTargetSizeChangeEvent = DispatchableEvent::SafeCast<RenderTargetSizeChangedEvent>(dispatchableEvent);

		if (renderTargetSizeChangeEvent == nullptr)
			return;

		resolutionTarget = renderTargetSizeChangeEvent->renderTargetSize;
	}

	void Camera::Init()
	{
		transform = OwningObject.lock()->GetComponent<Transform>();

		GuardAgainstNull(transform, "Failed to get Transform from OwningObject");
	}

	void Camera::Start()
	{

	}

	void Camera::Update(float deltaTime)
	{

	}

	void Camera::Draw()
	{

	}

	void Camera::SetZoom(float zoom)
	{
		this->zoom = std::max(0.01f, zoom); // Prevent negative or zero
	}

	void Camera::SetViewport(const SDL_FRect& normalizedViewport)
	{
		this->normalizedViewport = normalizedViewport;
	}

	float Camera::GetZoom() const
	{
		return zoom;
	}

	const SDL_FRect& Camera::GetViewport() const
	{
		return normalizedViewport;
	}

	SDL_Rect Camera::GetViewBox() const
	{
		return SDL_Rect
		{
			static_cast<int>(normalizedViewport.x * resolutionTarget.x),
			static_cast<int>(normalizedViewport.y * resolutionTarget.y),
			static_cast<int>(normalizedViewport.w * resolutionTarget.x),
			static_cast<int>(normalizedViewport.h * resolutionTarget.y)
		};
	}

	Vector2F Camera::WorldToScreen(const Vector2F& worldPos) const
	{
		auto cameraWorldPos = transform->GetWorldPosition();
		auto offset = worldPos - cameraWorldPos;

		auto viewport = GetViewBox();

		auto screenPosX = viewport.x + (offset.x * zoom) + viewport.w * 0.5f;
		auto screenPosY = viewport.y + (offset.y * zoom) + viewport.h * 0.5f;

		return Vector2F(screenPosX, screenPosY);
	}

	Vector2F Camera::ScreenToWorld(const Vector2F& screenPos) const
	{
		auto viewport = GetViewBox();

		auto localX = (screenPos.x - viewport.x - viewport.w * 0.5f) / zoom;
		auto localY = (screenPos.y - viewport.y - viewport.h * 0.5f) / zoom;

		return transform->GetWorldPosition() + Vector2(localX, localY);
	}

	bool Camera::IsVisible(const SDL_Rect& screenRect) const
	{
		auto viewBox = GetViewBox();

		return Collision::RectVsRect(viewBox, screenRect);
	}

	bool Camera::IsVisible(const Circle& circle) const
	{
		auto viewBox = GetViewBox();

		return Collision::CircleVsRect(circle, viewBox);
	}

	bool Camera::IsVisible(const Vector2F& p1, const Vector2F& p2) const
	{
		auto viewBox = GetViewBox();

		return Collision::SegmentVsRect(p1, p2, viewBox);
	}
}