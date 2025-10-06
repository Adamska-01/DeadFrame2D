#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <SDL_rect.h>


namespace DeadFrame2D::Engine
{
	class Camera;
	class Transform;


	class DF2D_API CameraFollow : public GameComponent
	{
	private:
		Camera* camera = nullptr;

		Transform* transform = nullptr;

		std::weak_ptr<GameObject> target;

		SDL_FRect worldBounds;

		DeadFrame2D::Core::Vector2F offset;

		DeadFrame2D::Core::Vector2I resolutionTarget;

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

		void SetOffset(DeadFrame2D::Core::Vector2F newOffset);

		void SetFollowSpeed(float speed);
	};
}