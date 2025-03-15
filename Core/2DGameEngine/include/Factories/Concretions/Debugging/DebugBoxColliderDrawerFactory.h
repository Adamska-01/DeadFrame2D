#pragma once
#include "Components/Collisions/BoxCollider2D.h"
#include "Factories/Abstractions/IFactoryWithInitialiser.h"


class DebugBoxColliderDrawerFactory : public IFactoryWithInitialiser<IDebugColliderDrawer<BoxCollider2D>*, BoxCollider2D*>
{
public:
	IDebugColliderDrawer<BoxCollider2D>* CreateProduct(BoxCollider2D* initialiser) override;
};