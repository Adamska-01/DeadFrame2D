#include "Core/SubSystems/Systems/Renderer.h"
#include "Factories/Concretions/Debugging/ColliderDrawerFactory.h"
#include "Factories/Products/Debugging/ColliderDrawer.h"


namespace DeadFrame2D::Factories
{
	b2Draw* ColliderDrawerFactory::CreateProduct()
	{
	#if DEBUG
		return new ColliderDrawer(DeadFrame2D::Core::Renderer::GetRenderer());
	#else
		return nullptr;
	#endif
	}
}
