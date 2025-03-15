#pragma once
#include "Components/Collisions/CircleCollider2D.h"
#include "Factories/Abstractions/IFactoryWithInitialiser.h"


class DebugCircleColliderDrawerFactory : public IFactoryWithInitialiser<IDebugColliderDrawer<CircleCollider2D>*, CircleCollider2D*>
{
public:
	IDebugColliderDrawer<CircleCollider2D>* CreateProduct(CircleCollider2D* initialiser) override;
};