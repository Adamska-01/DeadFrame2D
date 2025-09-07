#pragma once
#include "Core/Math/Vector2.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <SDL_rect.h>


class Camera;
class Transform;


class CameraFollow : public GameComponent
{
private:
	Camera* camera = nullptr;

	Transform* transform = nullptr;

	std::weak_ptr<GameObject> target;

	SDL_FRect worldBounds;

	Vector2F offset;

	Vector2I resolutionTarget;

	float followSpeed;


	void RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


public:
	CameraFollow(Camera* camera, std::weak_ptr<GameObject> target);
	
	virtual ~CameraFollow() override;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetTarget(std::weak_ptr<GameObject> newTarget);

	void SetBounds(const SDL_FRect& bounds);

	void SetOffset(Vector2F newOffset);

	void SetFollowSpeed(float speed);
};