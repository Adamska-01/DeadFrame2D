#pragma once
#include "Data/Systems/CoreContext.h"


namespace DF2D::Internal::CoreContextIterator
{
	template<typename Func>
	void ForEach(Data::CoreContext& ctx, Func&& func)
	{
		func(ctx.audioManager);
		func(ctx.coroutineScheduler);
		func(ctx.textureManager);
		func(ctx.input);
		func(ctx.physicsEngine);
		func(ctx.renderer);
		func(ctx.uiManager);
		func(ctx.window);
	}
}