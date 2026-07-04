#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
#include "Utilities/Debugging/Guards.h"
#include <typeindex>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	std::vector<Camera*> Camera::cameras = {};


	Camera::Camera()
		: normalizedViewport({ 0.0f, 0.0f, 1.0f, 1.0f }),
		renderTarget(0),
		zoom(1.0f)
	{
		cameras.push_back(this);

		EventDispatcher::RegisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), this, &Camera::RenderTargetSizeChangedEventHandler);
	}

	Camera::~Camera()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), this);

		if (renderTarget != 0 && renderer != nullptr)
		{
			renderer->DestroyTexture(renderTarget);
			renderTarget = 0;
		}

		std::erase(cameras, this);
	}

	void Camera::RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto renderTargetSizeChangeEvent = DispatchableEvent::SafeCast<RenderTargetSizeChangedEvent>(dispatchableEvent);

		if (renderTargetSizeChangeEvent == nullptr)
			return;

		resolutionTarget = renderTargetSizeChangeEvent->renderTargetSize;

		if (renderer == nullptr)
			return;

		if (renderTarget != 0)
		{
			renderer->DestroyTexture(renderTarget);
		}

		renderTarget = renderer->CreateRenderTarget(resolutionTarget.x, resolutionTarget.y);
	}

	void Camera::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));

		renderer = GetGameObject()->CoreContext().renderer;

		if (renderer != nullptr)
		{
			resolutionTarget = renderer->GetResolutionTarget();

			if (renderTarget == 0)
			{
				renderTarget = renderer->CreateRenderTarget(resolutionTarget.x, resolutionTarget.y);
			}
		}
	}

	void Camera::SetZoom(float zoom)
	{
		// Prevent negative or zero
		this->zoom = std::max(0.01f, zoom);
	}

	void Camera::SetViewport(const RectF& normalizedViewport)
	{
		this->normalizedViewport = normalizedViewport;
	}

	float Camera::GetZoom() const
	{
		return zoom;
	}

	const RectF& Camera::GetViewport() const
	{
		return normalizedViewport;
	}

	// TODO: Return Vector2F (?)
	RectF Camera::GetViewBox() const
	{
		return RectF
		{
			.x = 0.0f,
			.y = 0.0f,
			.w = static_cast<float>(resolutionTarget.x),
			.h = static_cast<float>(resolutionTarget.y)
		};
	}

	RectF Camera::GetNormalizedViewBox() const
	{
		return RectF
		{
			.x = normalizedViewport.x * resolutionTarget.x,
			.y = normalizedViewport.y * resolutionTarget.y,
			.w = normalizedViewport.w * resolutionTarget.x,
			.h = normalizedViewport.h * resolutionTarget.y
		};
	}

	TextureID Camera::GetRenderTarget() const
	{
		return renderTarget;
	}

	Vector2F Camera::WorldToScreen(const Vector2F& worldPos) const
	{
		auto cameraWorldPos = transform->GetWorldPosition();
		auto offset = worldPos - cameraWorldPos;

		auto screenPosX = (offset.x * zoom) + resolutionTarget.x * 0.5f;
		auto screenPosY = (offset.y * zoom) + resolutionTarget.y * 0.5f;

		return Vector2F(screenPosX, screenPosY);
	}

	Vector2F Camera::ScreenToWorld(const Vector2F& screenPos) const
	{
		auto localX = (screenPos.x - resolutionTarget.x * 0.5f) / zoom;
		auto localY = (screenPos.y - resolutionTarget.y * 0.5f) / zoom;

		return transform->GetWorldPosition() + Vector2(localX, localY);
	}

	const std::vector<Camera*>& Camera::GetCameras()
	{
		return cameras;
	}
}