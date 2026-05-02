#pragma once
#include "Data/Components/Animation/SpriteAnimationProperties.h"
#include "Data/Components/Animation/SpriteAnimationState.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <string>
#include <unordered_map>


namespace DeadFrame2D::Engine
{
	class Transform;
	class SpriteRenderer;


	class DF2D_API SpriteAnimator : public GameComponent
	{
		TYPE_INFO(SpriteAnimator, GameComponent);


	protected:
		ComponentHandle<Transform> transform;

		ComponentHandle<SpriteRenderer> sprite;

		std::unordered_map<std::string, DeadFrame2D::Data::SpriteAnimationProperties> animations;

		std::string currentAnimationID;

		DeadFrame2D::Data::SpriteAnimationState animState;

		DeadFrame2D::Data::RenderTask renderTask;


	public:
		SpriteAnimator();


		virtual void Init() override;

		virtual void Update(float dt) override;

		virtual void Draw() override;


		void AddAnimation(const DeadFrame2D::Data::SpriteAnimationProperties& properties);

		void PlayAnimation(const std::string& name, bool restartIfPlaying = false);

		bool IsPlaying(const std::string& name) const;

		void SetFlipState(SDL_RendererFlip flipState);

		float GetAnimationProgressRatio() const;

		const DeadFrame2D::Data::SpriteAnimationProperties* GetCurrentAnimationProperties() const;

		SDL_Rect GetFrameRect() const;
	};
}