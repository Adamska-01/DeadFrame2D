#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
#include "Utilities/Debugging/Guards.h"
#include "Utilities/Helpers/Events/EventHelpers.h"
#include <algorithm>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	std::vector<ComponentHandle<Camera>> Camera::cameras = {};


	Camera::Camera()
		: renderer(nullptr),
		normalizedViewport({ 0.0f, 0.0f, 1.0f, 1.0f }),
		renderTarget(0),
		zoom(1.0f),
		fitMode(CameraFitMode::ENVELOPE),
		referenceResolution(1920, 1080),
		viewScale(1.0f)
	{
	}

	Camera::~Camera()
	{
		if (renderTarget != 0 && renderer != nullptr)
		{
			renderer->DestroyTexture(renderTarget);
			renderTarget = 0;
		}

		// GetHandleAs<Camera>() can't be used here: by the time this destructor runs, the owning
		// unique_ptr has already nulled its internal pointer, so bucket-lookup-based handle
		// resolution fails. GetHandle() is a plain member copy with no bucket lookup, so it's safe.
		std::erase(cameras, ComponentHandle<Camera>(GetHandle()));
	}

	void Camera::RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto renderTargetSizeChangeEvent = DispatchableEvent::SafeCast<RenderTargetSizeChangedEvent>(dispatchableEvent);

		if (renderTargetSizeChangeEvent == nullptr)
			return;

		resolutionTarget = renderTargetSizeChangeEvent->renderTargetSize;

		RecomputeViewScale();

		if (renderer == nullptr)
			return;

		if (renderTarget != 0)
		{
			renderer->DestroyTexture(renderTarget);
		}

		renderTarget = renderer->CreateRenderTarget(resolutionTarget.x, resolutionTarget.y);
	}

	void Camera::RecomputeViewScale()
	{
		// ENVELOPE ignores the reference resolution, so the view scale stays at 1.
		if (fitMode == CameraFitMode::ENVELOPE || referenceResolution.x <= 0 || referenceResolution.y <= 0)
		{
			viewScale = 1.0f;

			return;
		}

		auto scaleX = static_cast<float>(resolutionTarget.x) / static_cast<float>(referenceResolution.x);
		auto scaleY = static_cast<float>(resolutionTarget.y) / static_cast<float>(referenceResolution.y);

		switch (fitMode)
		{
		case CameraFitMode::MATCH_WIDTH:
			viewScale = scaleX;
			break;

		case CameraFitMode::MATCH_HEIGHT:
			viewScale = scaleY;
			break;

		case CameraFitMode::LETTERBOX:
			// Use the smaller scale so the full reference view remains visible.
			viewScale = std::min(scaleX, scaleY);
			break;

		default:
			viewScale = 1.0f;
			break;
		}
	}

	void Camera::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));

		renderer = GetGameObject()->CoreContext().renderer;

		auto* eventDispatcher = Guard::AgainstNullAssignment(GetGameObject()->ServiceContext().eventDispatcher, NAME_OF(eventDispatcher));

		eventDispatcher->RegisterEventHandler<RenderTargetSizeChangedEvent>(GetHandle(), EventHelpers::BindFunction(this, &Camera::RenderTargetSizeChangedEventHandler));

		if (renderer != nullptr)
		{
			resolutionTarget = renderer->GetResolutionTarget();

			if (renderTarget == 0)
			{
				renderTarget = renderer->CreateRenderTarget(resolutionTarget.x, resolutionTarget.y);
			}
		}

		RecomputeViewScale();

		cameras.push_back(GetHandleAs<Camera>());
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

	void Camera::SetFitMode(CameraFitMode fitMode)
	{
		this->fitMode = fitMode;

		RecomputeViewScale();
	}

	void Camera::SetReferenceResolution(const Vector2I& referenceResolution)
	{
		this->referenceResolution = referenceResolution;

		RecomputeViewScale();
	}

	float Camera::GetZoom() const
	{
		return zoom;
	}

	CameraFitMode Camera::GetFitMode() const
	{
		return fitMode;
	}

	const Vector2I& Camera::GetReferenceResolution() const
	{
		return referenceResolution;
	}

	float Camera::GetViewScale() const
	{
		return viewScale;
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
		auto scale = zoom * viewScale;

		auto screenPosX = (offset.x * scale) + resolutionTarget.x * 0.5f;
		auto screenPosY = (offset.y * scale) + resolutionTarget.y * 0.5f;

		return Vector2F(screenPosX, screenPosY);
	}

	Vector2F Camera::ScreenToWorld(const Vector2F& screenPos) const
	{
		auto scale = zoom * viewScale;

		auto localX = (screenPos.x - resolutionTarget.x * 0.5f) / scale;
		auto localY = (screenPos.y - resolutionTarget.y * 0.5f) / scale;

		return transform->GetWorldPosition() + Vector2(localX, localY);
	}

	const std::vector<ComponentHandle<Camera>>& Camera::GetCameras()
	{
		return cameras;
	}
}