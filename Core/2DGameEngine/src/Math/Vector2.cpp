#include <cmath>
#include <Math/MathConstants.h>
#include <Math/Vector2.h>


const Vector2 Vector2::Zero(0.0f, 0.0f);

const Vector2 Vector2::One(1.0f, 1.0f);


Vector2::Vector2()
	: x(0), y(0)
{
}

Vector2::Vector2(float x, float y)
	: x(x), y(y)
{
}

float Vector2::Distance(const Vector2& other) const
{
	auto dx = other.x - x;
	auto dy = other.y - y;

	return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

float Vector2::Magnitude() const
{
	return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

Vector2& Vector2::Normalize()
{
	auto magnitude = Magnitude();

	// Avoid division by zero
	if (magnitude > 0.0)
	{
		x /= magnitude;
		y /= magnitude;
	}

	return *this;
}

float Vector2::AngleBetween(const Vector2 other) const
{
	auto angle = atan2(y - other.y, x - other.x) * (MathConstants::PI / 180);

	return angle >= 0 ? angle : 360 + angle;
}

float Vector2::Dot(const Vector2& other) const
{
	return (x * other.x) + (y * other.y);
}

float Vector2::Cross(const Vector2& other) const
{
	return (x * other.y) - (y * other.x);
}

bool Vector2::IsZero() const
{
	return (x == 0 && y == 0) ? true : false;
}

Vector2& Vector2::operator=(const Vector2& other)
{
	x = other.x;
	y = other.y;

	return *this;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(const Vector2& other) const
{
	return Vector2(x * other.x, y * other.y);
}

Vector2 Vector2::operator/(const Vector2& other) const
{
	return Vector2(x / other.x, y / other.y);
}

Vector2 Vector2::operator*(const float& scalar) const
{
	return Vector2(x * scalar, y * scalar);
}

Vector2& Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2& Vector2::operator*=(const Vector2& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

Vector2& Vector2::operator/=(const Vector2& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

bool Vector2::operator==(const Vector2& other) const
{
	if (this->x == other.x && this->y == other.y)
		return true;

	return false;
}