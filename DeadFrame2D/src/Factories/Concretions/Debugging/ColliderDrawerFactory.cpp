#include "Factories/Concretions/Debugging/ColliderDrawerFactory.h"
#include "Factories/Products/Debugging/ColliderDrawer.h"


namespace DeadFrame2D::Factories
{
	b2Draw* ColliderDrawerFactory::CreateProduct()
	{
	#if DEBUG
		return new ColliderDrawer();
	#else
		return nullptr;
	#endif
	}
}
