#pragma once
#include "Data/Services/ServiceContext.h"
#include "Data/Systems/CoreContext.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandleBase.h"


namespace DF2D::Engine
{
	/**
	 * @brief RAII construction-time context threaded from Scene::Instantiate into
	 * GameObject's constructor, so a GameObject can resolve its own handle and engine
	 * contexts (and therefore add components, spawn/parent children, etc.) while still
	 * under construction. Instances nest correctly (a constructor that itself
	 * instantiates another GameObject installs its own context and restores the
	 * previous one on return). Not for use outside Scene/GameObject.
	 */
	class DF2D_API GameObjectConstructionContext
	{
		friend class GameObject;


	private:
		// The TLS variable itself lives in the .cpp: MSVC does not allow a
		// thread_local class member to carry dll linkage (C2492).
		static const GameObjectConstructionContext* Current();

		static void SetCurrent(const GameObjectConstructionContext* ctx);


		const GameObjectConstructionContext* previous;

		ObjectHandleBase handle;

		Data::CoreContext coreCtx;

		Data::ServiceContext serviceCtx;


	public:
		GameObjectConstructionContext(ObjectHandleBase handle, Data::CoreContext coreCtx, Data::ServiceContext serviceCtx);

		~GameObjectConstructionContext();

		GameObjectConstructionContext(const GameObjectConstructionContext&) = delete;

		GameObjectConstructionContext& operator=(const GameObjectConstructionContext&) = delete;
	};
}