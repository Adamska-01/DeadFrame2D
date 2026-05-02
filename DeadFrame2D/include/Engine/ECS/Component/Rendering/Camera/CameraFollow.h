#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"
#include <SDL_rect.h>


namespace DF2D::Engine
{
	class Camera;
	class Transform;


	class DF2D_API CameraFollow : public GameComponent
	{
		TYPE_INFO(CameraFollow, GameComponent);


	private:
		ComponentHandle<Camera> camera;

		ComponentHandle<Transform> transform;

		ObjectHandle<GameObject> target;

		SDL_FRect worldBounds;

		Core::Vector2F offset;

		Core::Vector2I resolutionTarget;

		float followSpeed;


		void RenderTargetSizeChangedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	public:
		CameraFollow(ComponentHandle<Camera> camera, ObjectHandle<GameObject> target);

		virtual ~CameraFollow() override;


		virtual void Update(float deltaTime) override;


		void SetTarget(ObjectHandle<GameObject> newTarget);

		void SetBounds(const SDL_FRect& bounds);

		void SetOffset(Core::Vector2F newOffset);

		void SetFollowSpeed(float speed);
	};
}