#pragma once
#include <type_traits>


namespace DF2D::Core
{
	template<typename, typename = void>
	struct HasTypeInfo : std::false_type
	{
	};

	template<typename T>
	struct HasTypeInfo<T, std::void_t<decltype(T::StaticTypeInfo)>> : std::true_type
	{
	};
}