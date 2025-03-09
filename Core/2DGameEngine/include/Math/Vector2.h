#pragma once
#include <cmath>
#include <iostream>
#include <Math/MathConstants.h>


template <typename T>
class Vector2
{
public:
	T x;

	T y;


	static const Vector2<T> Zero;

	static const Vector2<T> One;


	Vector2() : x(0), y(0) { }

	Vector2(T x, T y) : x(x), y(y) { }
	
	virtual	~Vector2() = default;


	T Distance(const Vector2<T>& other) const;

	T Magnitude() const;
	
	T AngleBetween(const Vector2<T> other) const;

	T Dot(const Vector2<T>& other) const;	

	T Cross(const Vector2<T>& other) const;

	Vector2<T>& Normalize();
	
	bool IsZero() const;


	Vector2<T>& operator=(const Vector2<T>& other);

	Vector2<T> operator+(const Vector2<T>& other) const;
	
	Vector2<T> operator-(const Vector2<T>& other) const;
	
	Vector2<T> operator*(const Vector2<T>& other) const;
	
	Vector2<T> operator/(const Vector2<T>& other) const;

	Vector2<T> operator*(const T& scalar) const;

	Vector2<T>& operator+=(const Vector2<T>& other);
	
	Vector2<T>& operator-=(const Vector2<T>& other);
	
	Vector2<T>& operator*=(const Vector2<T>& other);
	
	Vector2<T>& operator/=(const Vector2<T>& other);

	bool operator==(const Vector2<T>& other) const;


	inline friend std::ostream& operator<<(std::ostream& out, const Vector2<T>& v)
	{
		out << "(" << v.x << ", " << v.y << ")" << std::endl;

		return out;
	}
};


// Static member initializations
template <typename T>
const Vector2<T> Vector2<T>::Zero = Vector2<T>(0, 0);

template <typename T>
const Vector2<T> Vector2<T>::One = Vector2<T>(1, 1);


// Specializations
using Vector2F = Vector2<float>;
using Vector2I = Vector2<int>;


template<typename T>
inline T Vector2<T>::Distance(const Vector2<T>& other) const
{
	return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

template<typename T>
inline T Vector2<T>::Magnitude() const
{
	return std::sqrt(x * x + y * y);
}

template<typename T>
inline T Vector2<T>::AngleBetween(const Vector2<T> other) const
{
	auto angle = atan2(y - other.y, x - other.x) * (MathConstants::PI / 180);

	return angle >= 0 ? angle : 360 + angle;
}

template<typename T>
inline T Vector2<T>::Dot(const Vector2<T>& other) const
{
	return (x * other.x) + (y * other.y);
}

template<typename T>
inline T Vector2<T>::Cross(const Vector2<T>& other) const
{
	return (x * other.y) - (y * other.x);
}

template<typename T>
inline Vector2<T>& Vector2<T>::Normalize()
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
inline bool Vector2<T>::IsZero() const
{
	return (x == 0 && y == 0) ? true : false;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator=(const Vector2<T>& other)
{
	x = other.x;
	y = other.y;

	return *this;
}

template<typename T>
inline Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
{
	return Vector2<T>(x + other.x, y + other.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const
{
	return Vector2<T>(x - other.x, y - other.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator*(const Vector2<T>& other) const
{
	return Vector2<T>(x * other.x, y * other.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator/(const Vector2<T>& other) const
{
	return Vector2<T>(x / other.x, y / other.y);
}

template<typename T>
inline Vector2<T> Vector2<T>::operator*(const T& scalar) const
{
	return Vector2<T>(x * scalar, y * scalar);
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator*=(const Vector2<T>& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

template<typename T>
inline bool Vector2<T>::operator==(const Vector2<T>& other) const
{
	if (this->x == other.x && this->y == other.y)
		return true;

	return false;
}