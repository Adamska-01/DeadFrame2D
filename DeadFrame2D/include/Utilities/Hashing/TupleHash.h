#pragma once
#include <Models/Input/InputDeviceType.h>
#include <string>
#include <tuple>


namespace DeadFrame2D::Utilities
{
	// https://www.variadic.xyz/post/0120-hashing-tuples/
	inline void hash_combine(std::size_t& seed, std::size_t value)
	{
		seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
	}


	template <class Tuple, std::size_t Index = std::tuple_size<Tuple>::value - 1>
	struct TupleHashImpl
	{
		static void apply(std::size_t& seed, const Tuple& tuple)
		{
			TupleHashImpl<Tuple, Index - 1>::apply(seed, tuple);

			hash_combine(
				seed, 
				std::hash<std::tuple_element_t<Index, Tuple>>{} (std::get<Index>(tuple)));
		}
	};

	// Base case
	template <class Tuple>
	struct TupleHashImpl<Tuple, 0>
	{
		static void apply(std::size_t& seed, const Tuple& tuple)
		{
			hash_combine(
				seed, 
				std::hash<std::tuple_element_t<0, Tuple>>{} (std::get<0>(tuple)));
		}
	};

	struct TupleHash
	{
		template <class... Args>
		std::size_t operator()(const std::tuple<Args...>& tuple) const noexcept
		{
			std::size_t seed = 0;

			TupleHashImpl<std::tuple<Args...>>::apply(seed, tuple);

			return seed;
		}
	};
};