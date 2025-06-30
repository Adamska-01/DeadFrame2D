#pragma once
#include "Factories/Abstractions/IFactory.h"


class b2Draw;


class ColliderDrawerFactory : public IFactory<b2Draw*>
{
public:
	b2Draw* CreateProduct() override;
};