#include "Engine/ECS/Component/UI/Abstractions/LayoutGroup.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	LayoutGroup::LayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
		: layoutSpacing(layoutSpacing), layoutPadding(layoutPadding)
	{
	}

	void LayoutGroup::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

		if (!gameObjEvent || gameObjEvent->gameObjectDestroyed == nullptr)
			return;

		auto target = gameObjEvent->gameObjectDestroyed;

		if (!target->IsChildOf(GetGameObject()))
			return;

		DeregisterAllHandlers(target);

		MarkDirty();
	}

	void LayoutGroup::OnChildActiveStateChangedHandler(const ObjectHandle<GameObject>& child, bool activeState)
	{
		MarkDirty();
	}

	void LayoutGroup::OnChildGameObjectAddedHandler(const ObjectHandle<GameObject>& obj)
	{
		MarkDirty();
	}

	void LayoutGroup::OnChildDestroyedHandler(const ObjectHandle<GameObject>& destroyedObj)
	{
		MarkDirty();
	}

	void LayoutGroup::Start()
	{
		UpdateLayout();
	}

	void LayoutGroup::Update(float deltaTime)
	{
		if (isDirty)
		{
			UpdateLayout();

			isDirty = false;
		}
	}

	void LayoutGroup::UpdateLayout()
	{
	}
}