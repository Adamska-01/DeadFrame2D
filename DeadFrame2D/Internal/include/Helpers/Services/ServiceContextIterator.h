#pragma once
#include "Data/Services/ServiceContext.h"


namespace DF2D::Internal::ServiceContextIterator
{
	template<typename Func>
	void ForEach(Data::ServiceContext& ctx, Func&& func)
	{
		func(ctx.eventManager);
		func(ctx.frameTimer);
		func(ctx.systemDialogBox);
		// sceneManager must be deleted before eventDispatcher (GameObjects might 
		// deregister events, thus eventDispatcher needs to be alive).
		func(ctx.sceneManager);
		func(ctx.eventDispatcher);
	}
}