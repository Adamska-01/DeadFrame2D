#pragma once
#include "Factories/Abstractions/IFactory.h"


class b2Draw;


namespace DeadFrame2D::Factories
{
	class ColliderDrawerFactory : public IFactory<b2Draw*>
	{
	public:
		b2Draw* CreateProduct() override;
	};
}