#pragma once
#include "Math/Vector2.h"
#include "GameComponent.h"


class Transform : public GameComponent
{
private:
	mutable Vector2F position = Vector2F::Zero;

	mutable Vector2F scale = Vector2F::One;
	
	mutable float rotation = 0.0f;

	Vector2F localPosition = Vector2F::Zero;
	
	Vector2F localScale = Vector2F::One;
	
	float localRotation = 0.0f;

	Vector2F startFramePosition;


	void RecalculateWorldTransform() const;


protected:
	virtual void MarkDirty() override;


public:
	Transform(Vector2F position = Vector2F::Zero, Vector2F scale = Vector2F::One, float angle = 0.0f);

	Transform(Transform&& other) = default;

	virtual ~Transform() override = default;


	void TranslateX(float x);
	
	void TranslateY(float y);
	
	void Translate(Vector2F v);

	void ScaleX(float x);
	
	void ScaleY(float y);
	
	void Scale(Vector2F v);

	void RotateByRadians(float radian);

	void RotateByDegrees(float degreeAngle);


	void Init() override;

	void Update(float dt) override;

	void Draw() override;


	Vector2F GetStartFramePosition() const;

	Vector2F GetLocalPosition() const;

	Vector2F GetWorldPosition() const;

	Vector2F GetLocalScale() const;

	Vector2F GetWorldScale() const;
	
	float GetLocalRotation() const;
	
	float GetWorldRotation() const;

	Vector2F GetForward() const;

	void SetLocalPosition(const Vector2F& pos);

	void SetWorldPosition(const Vector2F& worldPos);
	
	void SetLocalScale(const Vector2F& scale);

	void SetWorldScale(const Vector2F& worldScale);

	void SetLocalRotation(float rotation);

	void SetWorldRotation(float worldRotation);

	
	inline void Log(std::string msg = "") const
	{
		std::cout << msg << "(X Y) = (" << localPosition.x << " " << localPosition.y << ")" << std::endl;
	}
};