#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API Transform : public GameComponent
	{
		TYPE_INFO(Transform, GameComponent);


	private:
		mutable DeadFrame2D::Core::Vector2F position = DeadFrame2D::Core::Vector2F::Zero;

		mutable DeadFrame2D::Core::Vector2F scale = DeadFrame2D::Core::Vector2F::One;

		mutable float rotation = 0.0f;

		DeadFrame2D::Core::Vector2F localPosition = DeadFrame2D::Core::Vector2F::Zero;

		DeadFrame2D::Core::Vector2F localScale = DeadFrame2D::Core::Vector2F::One;

		float localRotation = 0.0f;

		DeadFrame2D::Core::Vector2F startFramePosition;


		void RecalculateWorldTransform() const;


	protected:
		virtual void MarkDirty() override;


	public:
		Transform(
			DeadFrame2D::Core::Vector2F position = DeadFrame2D::Core::Vector2F::Zero,
			DeadFrame2D::Core::Vector2F scale = DeadFrame2D::Core::Vector2F::One, 
			float angle = 0.0f);

		Transform(Transform&& other) = default;

		virtual ~Transform() override = default;


		void TranslateX(float x);

		void TranslateY(float y);

		void Translate(DeadFrame2D::Core::Vector2F v);

		void ScaleX(float x);

		void ScaleY(float y);

		void Scale(DeadFrame2D::Core::Vector2F v);

		void RotateByRadians(float radian);

		void RotateByDegrees(float degreeAngle);


		void Update(float dt) override;


		DeadFrame2D::Core::Vector2F GetStartFramePosition() const;

		DeadFrame2D::Core::Vector2F GetLocalPosition() const;

		DeadFrame2D::Core::Vector2F GetWorldPosition() const;

		DeadFrame2D::Core::Vector2F GetLocalScale() const;

		DeadFrame2D::Core::Vector2F GetWorldScale() const;

		float GetLocalRotation() const;

		float GetWorldRotation() const;

		DeadFrame2D::Core::Vector2F GetForward() const;

		void SetLocalPosition(const DeadFrame2D::Core::Vector2F& pos);

		void SetWorldPosition(const DeadFrame2D::Core::Vector2F& worldPos);

		void SetLocalScale(const DeadFrame2D::Core::Vector2F& scale);

		void SetWorldScale(const DeadFrame2D::Core::Vector2F& worldScale);

		void SetLocalRotation(float rotation);

		void SetWorldRotation(float worldRotation);


		inline void Log(std::string prefixMsg = "") const
		{
			std::cout << prefixMsg << "(X Y) = (" << localPosition.x << " " << localPosition.y << ")" << std::endl;
		}
	};
}