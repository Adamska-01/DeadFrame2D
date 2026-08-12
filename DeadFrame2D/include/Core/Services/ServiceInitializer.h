#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Services/Time/Abstractions/IFrameCycle.h"
#include "Data/Services/ServiceContext.h"
#include "DF2D_API.h"
#include "Models/SystemConfig.h"


namespace DF2D::Core
{
	class DF2D_API ServiceInitializer
	{
	private:
		Data::ServiceContext ctx;


	public:
		ServiceInitializer(Models::SystemConfig config);

		~ServiceInitializer();


		Data::ServiceContext GetServiceContext() const;
	};
}