#include "Factories/Concretions/Debugging/DebugBoxColliderDrawerFactory.h"
#include "Factories/Products/Debugging/DebugBoxColliderDrawer.h"
#include "Factories/Products/Debugging/EmptyColliderDrawer.h"


IDebugColliderDrawer<BoxCollider2D>* DebugBoxColliderDrawerFactory::CreateProduct(BoxCollider2D* initialiser)
{
#if DEBUG
	return new DebugBoxColliderDrawer(initialiser);
#else
	return new EmptyColliderDrawer<BoxCollider2D>();
#endif
}