#pragma once


namespace DF2D::Core
{
	template<typename T>
	struct Rect
	{
		T x = 0;

		T y = 0;

		T w = 0;

		T h = 0;


		constexpr bool operator==(const Rect<T>& other) const;

		constexpr bool operator!=(const Rect<T>& other) const;


		constexpr bool Contains(T px, T py) const;

		constexpr bool IsEmpty() const;
	};
}


namespace DF2D::Core
{
	using RectF = Rect<float>;
	using RectI = Rect<int>;


	template<typename T>
	inline constexpr bool Rect<T>::operator==(const Rect<T>& other) const
	{
		return x == other.x && y == other.y && w == other.w && h == other.h;
	}

	template<typename T>
	inline constexpr bool Rect<T>::operator!=(const Rect<T>& other) const
	{
		return !(*this == other);
	}


	template<typename T>
	inline constexpr bool Rect<T>::Contains(T px, T py) const
	{
		return px >= x && px < x + w && py >= y && py < y + h;
	}

	template<typename T>
	inline constexpr bool Rect<T>::IsEmpty() const
	{
		return w <= 0 || h <= 0;
	}
}