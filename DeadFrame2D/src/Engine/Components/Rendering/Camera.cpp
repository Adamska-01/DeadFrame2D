#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Engine/Components/Rendering/Camera.h"
#include "Engine/Components/Transform.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Renderer/RenderTargetSizeChangedEvent.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include "Utilities/Debugging/Guards.h"
#include <typeindex>


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Utilities;


	std::vector<Camera*> Camera::cameras = {};


	Camera::Camera()
		: normalizedViewport({ 0.0f, 0.0f, 1.0f, 1.0f }),
		zoom(1.0f)
	{
		cameras.push_back(this);

		resolutionTarget = Renderer::GetResolutionTarget();

		renderTarget = SDL_CreateTexture(Renderer::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, resolutionTarget.x, resolutionTarget.y);

		EventDispatcher::RegisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), this, &Camera::RenderTargetSizeChangedEventHandler);
	}

	Camera::~Camera()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(RenderTargetSizeChangedEvent)), this);

		std::erase(cameras, this);
	}

	void Camera::RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto renderTargetSizeChangeEvent = DispatchableEvent::SafeCast<RenderTargetSizeChangedEvent>(dispatchableEvent);

		if (renderTargetSizeChangeEvent == nullptr)
			return;

		resolutionTarget = renderTargetSizeChangeEvent->renderTargetSize;

		if (renderTarget != nullptr)
		{
			SDL_DestroyTexture(renderTarget);
		}

		renderTarget = SDL_CreateTexture(Renderer::GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, resolutionTarget.x, resolutionTarget.y);
	}

	void Camera::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));
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

	SDL_FRect Camera::GetViewBox() const
	{
		return SDL_FRect
		{
			.x = 0.0f,
			.y = 0.0f,
			.w = static_cast<float>(resolutionTarget.x),
			.h = static_cast<float>(resolutionTarget.y)
		};
	}

	SDL_FRect Camera::GetNormalizedViewBox() const
	{
		return SDL_FRect
		{
			.x = normalizedViewport.x * resolutionTarget.x,
			.y = normalizedViewport.y * resolutionTarget.y,
			.w = normalizedViewport.w * resolutionTarget.x,
			.h = normalizedViewport.h * resolutionTarget.y
		};
	}

	SDL_Texture* Camera::GetRenderTarget()
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