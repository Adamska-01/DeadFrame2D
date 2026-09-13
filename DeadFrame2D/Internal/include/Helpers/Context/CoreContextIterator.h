#pragma once
#include "Data/Systems/CoreContext.h"


namespace DF2D::Internal::CoreContextIterator
{
	template<typename Func>
	void ForEach(Data::CoreContext& ctx, Func&& func)
	{
		// Order here matters! (destruction order)
		func(ctx.audioManager);
		func(ctx.coroutineScheduler);
		func(ctx.input);
		func(ctx.physicsEngine);
		func(ctx.uiManager);
		func(ctx.textureManager);
		func(ctx.renderer);
		func(ctx.window);
	}
}