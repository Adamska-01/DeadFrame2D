#pragma once
#include <iostream>


class Vector2
{
public:
	float x;

	float y;


	static const Vector2 Zero;

	static const Vector2 One;


	Vector2();

	Vector2(float x, float y);
	
	~Vector2() {}


	float Distance(const Vector2& other) const;
	
	float Magnitude() const;
	
	Vector2& Normalize();

	float AngleBetween(const Vector2 other) const;
	
	float Dot(const Vector2& other) const;

	float Cross(const Vector2& other) const;
	
	bool IsZero() const;


	Vector2& operator=(const Vector2& other);

	Vector2 operator+(const Vector2& other) const;
	
	Vector2 operator-(const Vector2& other) const;
	
	Vector2 operator*(const Vector2& other) const;
	
	Vector2 operator/(const Vector2& other) const;

	Vector2 operator*(const float& scalar) const;

	Vector2& operator+=(const Vector2& other);
	
	Vector2& operator-=(const Vector2& other);
	
	Vector2& operator*=(const Vector2& other);
	
	Vector2& operator/=(const Vector2& other);

	bool operator==(const Vector2& other) const;


	inline friend std::ostream& operator<<(std::ostream& out, const Vector2& v)
	{
		out << "(" << v.x << ", " << v.y << ")" << std::endl;

		return out;
	}
};