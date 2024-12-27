#pragma once
#include "Math/Vector2.h"
#include "GameComponent.h"


class Transform : public GameComponent
{
public:
	Transform(Vector2 position = Vector2::Zero, Vector2 scale = Vector2::One, float angle = 0.0f);

	Transform(Transform&& other) = default;

	~Transform() override;


	Vector2 position;

	Vector2 scale;
	
	float angle;


	void TranslateX(float x);
	
	void TranslateY(float y);
	
	void Translate(Vector2 v);

	void ScaleX(float x);
	
	void ScaleY(float y);
	
	void Scale(Vector2 v);

	void RotateBy(float angle);


	void Init() override;

	void Update(float dt) override;

	void Draw() override;

	
	inline void Log(std::string msg = "") const
	{
		std::cout << msg << "(X Y) = (" << position.x << " " << position.y << ")" << std::endl;
	}


protected:
	virtual void Clean() override;
};