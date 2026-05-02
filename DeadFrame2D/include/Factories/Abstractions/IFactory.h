#pragma once
#include "DF2D_API.h"


namespace DF2D::Factories
{
	template<typename T>
	class DF2D_API IFactory
	{
	public:
		virtual ~IFactory() = default;


		virtual T CreateProduct() = 0;
	};
}