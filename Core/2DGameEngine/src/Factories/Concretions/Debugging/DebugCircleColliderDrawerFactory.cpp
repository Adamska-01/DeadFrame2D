#include "Factories/Concretions/Debugging/DebugCircleColliderDrawerFactory.h"
#include "Factories/Products/Debugging/DebugCircleColliderDrawer.h"
#include "Factories/Products/Debugging/EmptyColliderDrawer.h"


IDebugColliderDrawer<CircleCollider2D>* DebugCircleColliderDrawerFactory::CreateProduct(CircleCollider2D* initialiser)
{
#if DEBUG
	return new DebugCircleColliderDrawer(initialiser);
#else
	return new EmptyColliderDrawer<CircleCollider2D>();
#endif
}