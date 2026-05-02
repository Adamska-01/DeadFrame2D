#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"
#include <memory>
#include <SDL.h>
#include <vector>


struct SDL_Texture;

namespace DF2D::Core
{
	struct Circle;
	class DeadFrameRuntime;
}


namespace DF2D::Engine
{
	class Transform;


	class DF2D_API Camera : public GameComponent
	{
		TYPE_INFO(Camera, GameComponent);


	private:
		static std::vector<Camera*> cameras;


		ComponentHandle<Transform> transform;

		Core::Vector2I resolutionTarget;

		SDL_FRect normalizedViewport;
		
		SDL_Texture* renderTarget = nullptr;

		float zoom;


		void RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	public:
		Camera();

		virtual ~Camera() override;


		virtual void Init() override;


		void SetZoom(float zoom);

		void SetViewport(const SDL_FRect& normalizedViewport);

		float GetZoom() const;

		/** @brief Sets the normalized viewport (0–1 range). */
		const SDL_FRect& GetViewport() const;

		/** @brief Returns the screen-space viewbox for this camera. */
		SDL_FRect GetViewBox() const;

		/** @brief Returns the screen-space normalized viewbox for this camera. */
		SDL_FRect GetNormalizedViewBox() const;

		SDL_Texture* GetRenderTarget();

		/** @brief Converts world position to screen position. */
		Core::Vector2F WorldToScreen(const Core::Vector2F& worldPos) const;

		/** @brief Converts screen position to world position. */
		Core::Vector2F ScreenToWorld(const Core::Vector2F& screenPos) const;


		static const std::vector<Camera*>& GetCameras();
	};
}