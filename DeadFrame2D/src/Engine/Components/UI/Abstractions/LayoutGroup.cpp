#include "Engine/Components/UI/Abstractions/LayoutGroup.h"
#include "Engine/Components/UI/Abstractions/UIComponent.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Utilities/Helpers/Events/EventHelpers.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	LayoutGroup::LayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
		: layoutSpacing(layoutSpacing), layoutPadding(layoutPadding)
	{
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), this, &LayoutGroup::GameObjectCreatedHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), this, &LayoutGroup::GameObjectDestroyedHandler);
	}

	LayoutGroup::~LayoutGroup()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), this);
	}

	void LayoutGroup::GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectCreatedEvent>(dispatchableEvent);

		if (gameObjEvent == nullptr || gameObjEvent->GetGameObject() == nullptr)
			return;
	
		auto& target = gameObjEvent->GetGameObject();

		if (!target->IsChildOf(GetGameObject()))
			return;

		DeregisterAllHandlers(target);
		RegisterAllHandlers(target);

		MarkDirty();
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

	void LayoutGroup::OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& child, bool activeState)
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
		}
	}

	void LayoutGroup::UpdateLayout()
	{
		for (const auto& ui : GetGameObject()->GetComponentsInChildren<UIComponent>())
		{
			DeregisterAllHandlers(ui->GetGameObject());
			RegisterAllHandlers(ui->GetGameObject());
		}

		isDirty = false;
	}
}