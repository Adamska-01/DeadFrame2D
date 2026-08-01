#include "Factories/Concretions/Context/Systems/Physics/PhysicsBackendFactory.h"
#include "Factories/Products/Context/Systems/Physics/Box2DPhysicsBackend.h"


namespace DF2D::Factories
{
	using namespace DF2D::Core;
	using namespace DF2D::Internal;
	using namespace DF2D::Models;


	std::unique_ptr<IPhysicsBackend> PhysicsBackendFactory::CreateProduct(PhysicsConfig config)
	{
		return std::make_unique<Box2DPhysicsBackend>(config);
	}
}