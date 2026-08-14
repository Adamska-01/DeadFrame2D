#pragma once
#include "Data/Services/ServiceContext.h"
#include "Data/Systems/CoreContext.h"
#include "DF2D_API.h"


namespace DF2D::Engine
{
	/**
	 * @brief Interface providing access to engine-wide Core and Service contexts.
	 *
	 * This interface exposes read-only access to the engine subsystems grouped
	 * into CoreContext and ServiceContext. The contexts are assigned internally
	 * by the engine (e.g., Scene) and are not meant to be modified externally.
	 */
	class DF2D_API IGameContextProvider
	{
	protected:
		void SetContexts(Data::CoreContext coreCtx, Data::ServiceContext serviceCtx);


		Data::CoreContext coreCtx;

		Data::ServiceContext serviceCtx;


	public:
		virtual ~IGameContextProvider() = default;


		const Data::CoreContext& CoreContext() const;

		const Data::ServiceContext& ServiceContext() const;
	};
}