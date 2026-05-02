#pragma once
#include "DF2D_API.h"


namespace DF2D::Factories
{
	template<typename T, typename U>
	class DF2D_API IFactoryWithInitialiser
	{
	public:
		virtual ~IFactoryWithInitialiser() = default;


		virtual T CreateProduct(U initialiser) = 0;
	};
}