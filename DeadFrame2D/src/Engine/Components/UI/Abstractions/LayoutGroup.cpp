#include "Engine/Components/UI/Abstractions/LayoutGroup.h"
#include "Engine/Components/UI/Abstractions/UIComponent.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Utilities/Helpers/Events/EventHelpers.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	LayoutGroup::LayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
		: layoutSpacing(layoutSpacing), layoutPadding(layoutPadding)
	{
		// TODO: Fucking remove this! Add proper events to the GameObjectNotifier
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), this, &LayoutGroup::GameObjectDestroyedHandler);
	}

	LayoutGroup::~LayoutGroup()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), this);
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