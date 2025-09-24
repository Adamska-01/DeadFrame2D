#pragma once
#include "Core/Math/Vector2.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <memory>
#include <SDL.h>
#include <vector>


namespace DeadFrame2D::Core
{
	struct Circle;
	class Engine;
}


namespace DeadFrame2D::Engine
{
	class Transform;


	class Camera : public GameComponent
	{
		friend class DeadFrame2D::Core::Engine;


	private:
		static std::vector<Camera*> cameras;


		Transform* transform;

		DeadFrame2D::Core::Vector2I resolutionTarget;

		SDL_FRect normalizedViewport;

		float zoom;


		void RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	public:
		Camera();

		virtual ~Camera() override;


		virtual void Init() override;

		virtual void Start() override;

		virtual void Update(float deltaTime) override;

		virtual void Draw() override;


		void SetZoom(float zoom);
	
		void SetViewport(const SDL_FRect& normalizedViewport);
	
		float GetZoom() const;

		/** @brief Sets the normalized viewport (0–1 range). */
		const SDL_FRect& GetViewport() const;

		/** @brief Returns the screen-space viewbox for this camera. */
		SDL_Rect GetViewBox() const;

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
	};
}