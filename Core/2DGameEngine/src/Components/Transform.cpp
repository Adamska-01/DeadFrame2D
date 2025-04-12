#include <Components/Transform.h>
#include <Math/MathConstants.h>


Transform::Transform(Vector2F position, Vector2F scale, float angle)
	: position(position), scale(scale), angle(angle)
{
}

Transform::~Transform()
{
}

void Transform::TranslateX(float x)
{
	position.x += x;
}

void Transform::TranslateY(float y)
{
	position.y += y;
}

void Transform::Translate(Vector2F v)
{
	position.x += v.x; 
	position.y += v.y;
}

void Transform::ScaleX(float x)
{
	scale.x = x;
}

void Transform::ScaleY(float y)
{
	scale.y = y;
}

void Transform::Scale(Vector2F v)
{
	scale.x = v.x;
	scale.y = v.y;
}

void Transform::RotateByRadians(float radian)
{
	this->angle += radian * (180.0f / MathConstants::PI);
}

void Transform::RotateByDegrees(float degreeAngle)
{
	this->angle += degreeAngle;
}

void Transform::Init()
{
}

void Transform::Update(float dt)
{
	startFramePosition = position;
}

void Transform::Draw()
{
}

Vector2F Transform::GetStartFramePosition() const
{
	return startFramePosition;
}