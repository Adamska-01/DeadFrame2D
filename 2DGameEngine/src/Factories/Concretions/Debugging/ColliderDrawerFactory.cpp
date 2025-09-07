#include "Core/SubSystems/Systems/Renderer.h"
#include "Factories/Concretions/Debugging/ColliderDrawerFactory.h"
#include "Factories/Products/Debugging/ColliderDrawer.h"


b2Draw* ColliderDrawerFactory::CreateProduct()
{
#if DEBUG
	return new ColliderDrawer(Renderer::GetRenderer());
#else
	return nullptr;
#endif
}
