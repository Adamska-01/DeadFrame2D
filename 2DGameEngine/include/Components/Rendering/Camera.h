#pragma once
#include "Components/GameComponent.h"
#include "EventSystem/DispatchableEvent.h"
#include "Math/Vector2.h"
#include <memory>
#include <SDL.h>
#include <vector>


class Transform;


class Camera : public GameComponent
{
	friend class Engine;


private:
	static std::vector<Camera*> cameras;


	Transform* transform;

	Vector2I resolutionTarget;

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
	Vector2F WorldToScreen(const Vector2F& worldPos) const;

	/** @brief Converts screen position to world position. */
	Vector2F ScreenToWorld(const Vector2F& screenPos) const;
};