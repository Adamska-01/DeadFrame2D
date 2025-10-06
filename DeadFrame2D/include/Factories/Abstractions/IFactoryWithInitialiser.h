#pragma once
#include "DF2D_API.h"


namespace DeadFrame2D::Factories
{
	template<typename T, typename U>
	class DF2D_API IFactoryWithInitialiser
	{
	public:
		virtual ~IFactoryWithInitialiser() = default;


		virtual T CreateProduct(U initialiser) = 0;
	};
}