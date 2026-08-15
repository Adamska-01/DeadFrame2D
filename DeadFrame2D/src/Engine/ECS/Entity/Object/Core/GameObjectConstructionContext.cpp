#include "Engine/ECS/Entity/Object/Core/GameObjectConstructionContext.h"
#include <utility>


namespace DF2D::Engine
{
	namespace
	{
		thread_local const GameObjectConstructionContext* g_current = nullptr;
	}


	const GameObjectConstructionContext* GameObjectConstructionContext::Current()
	{
		return g_current;
	}

	void GameObjectConstructionContext::SetCurrent(const GameObjectConstructionContext* ctx)
	{
		g_current = ctx;
	}


	GameObjectConstructionContext::GameObjectConstructionContext(ObjectHandleBase handle, Data::CoreContext coreCtx, Data::ServiceContext serviceCtx)
		: previous(Current()),
		handle(std::move(handle)),
		coreCtx(coreCtx),
		serviceCtx(serviceCtx)
	{
		SetCurrent(this);
	}

	GameObjectConstructionContext::~GameObjectConstructionContext()
	{
		SetCurrent(previous);
	}
}