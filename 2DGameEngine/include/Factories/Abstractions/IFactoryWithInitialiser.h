#pragma once


namespace DeadFrame2D::Factories
{
	template<typename T, typename U>
	class IFactoryWithInitialiser
	{
	public:
		virtual ~IFactoryWithInitialiser() = default;


		virtual T CreateProduct(U initialiser) = 0;
	};
}