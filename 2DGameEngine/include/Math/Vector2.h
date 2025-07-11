#pragma once
#include "Math/MathConstants.h"
#include <cmath>
#include <iostream>
#include <tuple>


template <typename T>
class Vector2
{
public:
	T x;

	T y;


	static const Vector2<T> Zero;

	static const Vector2<T> One;

	static const Vector2<T> Up;

	static const Vector2<T> Down;
	
	static const Vector2<T> Left;
	
	static const Vector2<T> Right;


	Vector2() : x(0), y(0) { }

	Vector2(T x, T y) : x(x), y(y) { }
	
	virtual	~Vector2() = default;


	constexpr T Distance(const Vector2<T>& other) const;

	constexpr T Magnitude() const;
	
	constexpr T AngleBetween(const Vector2<T> other) const;

	constexpr T Dot(const Vector2<T>& other) const;

	constexpr T Cross(const Vector2<T>& other) const;
	
	constexpr Vector2<T> Min(const Vector2<T>& other) const;
	
	constexpr Vector2<T> Max(const Vector2<T>& other) const;
	
	constexpr Vector2<T> Rotated(float angleInDegrees) const;

	constexpr Vector2<T>& Normalize();
	
	constexpr bool IsZero() const;


	static constexpr T Dot(const Vector2<T>& a, const Vector2<T>& b);


	constexpr Vector2<T>& operator=(const Vector2<T>& other);

	constexpr Vector2<T> operator+(const Vector2<T>& other) const;

	constexpr Vector2<T> operator-(const Vector2<T>& other) const;

	constexpr Vector2<T> operator*(const Vector2<T>& other) const;

	constexpr Vector2<T> operator/(const Vector2<T>& other) const;

	constexpr Vector2<T> operator*(const T& scalar) const;

	constexpr Vector2<T> operator/(const T& scalar) const;

	constexpr Vector2<T>& operator*=(const T& scalar);

	constexpr Vector2<T>& operator/=(const T& scalar);

	constexpr Vector2<T>& operator+=(const Vector2<T>& other);

	constexpr Vector2<T>& operator-=(const Vector2<T>& other);

	constexpr Vector2<T>& operator*=(const Vector2<T>& other);

	constexpr Vector2<T>& operator/=(const Vector2<T>& other);

	constexpr bool operator==(const Vector2<T>& other) const;

	// Allow this to be used in an ordered map as key
	constexpr bool operator<(const Vector2<T>& other) const; 


	inline friend std::ostream& operator<<(std::ostream& out, const Vector2<T>& v)
	{
		out << "(" << v.x << ", " << v.y << ")";

		return out;
	}
};


// Static member initializations
template <typename T>
const Vector2<T> Vector2<T>::Zero = Vector2<T>(0, 0);

template <typename T>
const Vector2<T> Vector2<T>::One = Vector2<T>(1, 1);

template <typename T>
const Vector2<T> Vector2<T>::Up = Vector2<T>(0, -1);

template <typename T>
const Vector2<T> Vector2<T>::Down = Vector2<T>(0, 1);

template <typename T>
const Vector2<T> Vector2<T>::Left = Vector2<T>(-1, 0);

template <typename T>
const Vector2<T> Vector2<T>::Right = Vector2<T>(1, 0);


// Specializations
using Vector2F = Vector2<float>;
using Vector2I = Vector2<int>;


template<typename T>
inline constexpr T Vector2<T>::Distance(const Vector2<T>& other) const
{
	return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

template<typename T>
inline constexpr T Vector2<T>::Magnitude() const
{
	return std::sqrt(x * x + y * y);
}

template<typename T>
inline constexpr T Vector2<T>::AngleBetween(const Vector2<T> other) const
{
	auto angle = atan2(y - other.y, x - other.x) * (MathConstants::PI / 180);

	return angle >= 0 ? angle : 360 + angle;
}

template<typename T>
inline constexpr T Vector2<T>::Dot(const Vector2<T>& other) const
{
	return (x * other.x) + (y * other.y);
}

template<typename T>
inline constexpr T Vector2<T>::Cross(const Vector2<T>& other) const
{
	return (x * other.y) - (y * other.x);
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::Min(const Vector2<T>& other) const
{
	return Vector2<T>(std::min(x, other.x), std::min(y, other.y));
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::Max(const Vector2<T>& other) const
{
	return Vector2<T>(std::max(x, other.x), std::max(y, other.y));
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::Rotated(float angleInDegrees) const
{
	float radians = angleInDegrees * MathConstants::PI / 180.0f;
	float cosA = std::cos(radians);
	float sinA = std::sin(radians);

	return Vector2<T>(
		static_cast<T>(x * cosA - y * sinA),
		static_cast<T>(x * sinA + y * cosA));
}

template<typename T>
inline constexpr Vector2<T>& Vector2<T>::Normalize()
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

template<typename T>
inline constexpr bool Vector2<T>::IsZero() const
{
	return *this == Vector2<T>::Zero;
}

template<typename T>
inline constexpr T Vector2<T>::Dot(const Vector2<T>& a, const Vector2<T>& b)
{
	return a.x * b.x + a.y * b.y;
}

template<typename T>
inline constexpr Vector2<T>& Vector2<T>::operator=(const Vector2<T>& other)
{
	x = other.x;
	y = other.y;

	return *this;
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
{
	return Vector2<T>(x + other.x, y + other.y);
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const
{
	return Vector2<T>(x - other.x, y - other.y);
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator*(const Vector2<T>& other) const
{
	return Vector2<T>(x * other.x, y * other.y);
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator/(const Vector2<T>& other) const
{
	return Vector2<T>(x / other.x, y / other.y);
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator*(const T& scalar) const
{
	return Vector2<T>(x * scalar, y * scalar);
}

template<typename T>
inline constexpr Vector2<T> Vector2<T>::operator/(const T& scalar) const
{
	return Vector2<T>(x / scalar, y / scalar);
}

template<typename T>
inline constexpr Vector2<T>& Vector2<T>::operator*=(const T& scalar)
{
	x *= scalar;
	y *= scalar;

	return *this;
}

template<typename T>
inline constexpr Vector2<T>& Vector2<T>::operator/=(const T& scalar)
{
	x /= scalar;
	y /= scalar;

	return *this;
}

template<typename T>
inline constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

template<typename T>
inline constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

template<typename T>
inline constexpr Vector2<T>& Vector2<T>::operator*=(const Vector2<T>& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

template<typename T>
inline constexpr Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

template<typename T>
inline constexpr bool Vector2<T>::operator==(const Vector2<T>& other) const
{
	return this->x == other.x && this->y == other.y;
}

template<typename T>
inline constexpr bool Vector2<T>::operator<(const Vector2<T>& other) const
{
	return std::tie(x, y) < std::tie(other.x, other.y);
}


namespace std 
{
	template<typename T>
	struct hash<Vector2<T>> 
	{
		std::size_t operator()(const Vector2<T>& v) const noexcept 
		{
			std::size_t h1 = std::hash<T>{}(v.x);
			std::size_t h2 = std::hash<T>{}(v.y);

			return h1 ^ (h2 << 1);
		}
	};
}