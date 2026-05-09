#include "Engine/ECS/Entity/Object/Core/IGameContextProvider.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	void IGameContextProvider::SetContexts(Data::CoreContext coreCtx, Data::ServiceContext serviceCtx)
	{
		this->coreCtx = coreCtx;
		this->serviceCtx = serviceCtx;
	}

	const Data::CoreContext& IGameContextProvider::CoreContext() const
	{
		return coreCtx;
	}

	const Data::ServiceContext& IGameContextProvider::ServiceContext() const
	{
		return serviceCtx;
	}
}