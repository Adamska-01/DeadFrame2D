#pragma once
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsBackend.h"
#include "DF2D_API.h"
#include "Factories/Abstractions/IFactoryWithInitialiser.h"
#include "Models/Physics/PhysicsConfig.h"
#include <memory>


namespace DF2D::Factories
{
	class DF2D_API PhysicsBackendFactory : public IFactoryWithInitialiser<std::unique_ptr<Core::IPhysicsBackend>, Models::PhysicsConfig>
	{
	public:
		std::unique_ptr<Core::IPhysicsBackend> CreateProduct(Models::PhysicsConfig config) override;
	};
}