#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/Entity/ComponentHandle.h"
#include <memory>
#include <SDL.h>
#include <vector>


struct SDL_Texture;

namespace DeadFrame2D::Core
{
	struct Circle;
	class DeadFrameRuntime;
}


namespace DeadFrame2D::Engine
{
	class Transform;


	class DF2D_API Camera : public GameComponent
	{
		TYPE_INFO(Camera, GameComponent);


	private:
		static std::vector<Camera*> cameras;


		ComponentHandle<Transform> transform;

		DeadFrame2D::Core::Vector2I resolutionTarget;

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
		SDL_Rect GetViewBox() const;

		/** @brief Returns the screen-space normalized viewbox for this camera. */
		SDL_Rect GetNormalizedViewBox() const;

		SDL_Texture* GetRenderTarget();

		/** @brief Converts world position to screen position. */
		DeadFrame2D::Core::Vector2F WorldToScreen(const DeadFrame2D::Core::Vector2F& worldPos) const;

		/** @brief Converts screen position to world position. */
		DeadFrame2D::Core::Vector2F ScreenToWorld(const DeadFrame2D::Core::Vector2F& screenPos) const;

		/**
		 * @brief Checks if the given screen-space rectangle is visible within this camera's view box.
		 * @param screenRect The non-rotated rectangle in screen coordinates.
		 * @return True if any part of the rectangle is inside the camera's viewport.
		 */
		bool IsVisible(const SDL_Rect& screenRect) const;

		/**
		 * @brief Checks if the given screen-space rectangle is visible within this camera's view box.
		 * @param circle The circle in screen coordinates.
		 * @return True if any part of the circle is inside the camera's viewport.
		 */
		bool IsVisible(const DeadFrame2D::Core::Circle& circle) const;

		/**
		 * @brief Checks if the given screen-space line segment is visible within this camera's view box.
		 * @param p1 The first endpoint of the line segment in screen coordinates.
		 * @param p2 The second endpoint of the line segment in screen coordinates.
		 * @return True if any part of the line segment is inside the camera's viewport.
		 */
		bool IsVisible(const DeadFrame2D::Core::Vector2F& p1, const DeadFrame2D::Core::Vector2F& p2) const;


		static const std::vector<Camera*>& GetCameras();
	};
}