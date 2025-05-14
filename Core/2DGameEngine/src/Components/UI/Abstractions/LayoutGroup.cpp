#include "Components/UI/Abstractions/LayoutGroup.h"
#include "EventSystem/DispatchableEvent.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectCreatedEvent.h"
#include "EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h"
#include "Tools/Helpers/EventHelpers.h"
#include "Components/UI/Abstractions/UIComponent.h"


LayoutGroup::LayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
	: layoutSpacing(layoutSpacing), layoutPadding(layoutPadding)
{
	auto identifier = reinterpret_cast<uintptr_t>(this);
	
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &LayoutGroup::GameObjectCreatedHandler), identifier);
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &LayoutGroup::GameObjectDestroyedHandler), identifier);
}

LayoutGroup::~LayoutGroup()
{
	auto identifier = reinterpret_cast<uintptr_t>(this);

	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), identifier);
	EventDispatcher::DeregisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), identifier);
}

void LayoutGroup::GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectCreatedEvent>(dispatchableEvent);

	if (gameObjEvent == nullptr || gameObjEvent->gameObjectCreated == nullptr)
		return;
	
	auto& target = gameObjEvent->gameObjectCreated; 

	if (!target->IsChildOf(OwningObject))
		return;

	DeregisterAllHandlers(target);
	RegisterAllHandlers(target);

	MarkDirty();
}

void LayoutGroup::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

	if (!gameObjEvent || gameObjEvent->gameObjectDestroyed.lock() == nullptr)
		return;

	auto target = gameObjEvent->gameObjectDestroyed;

	if (!target.lock()->IsChildOf(OwningObject))
		return;

	DeregisterAllHandlers(target);

	MarkDirty();
}

void LayoutGroup::OnGameObjectActiveStateChangedHandler(GameObject* child, bool activeState)
{
	MarkDirty();
}

void LayoutGroup::Init()
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

void LayoutGroup::Draw()
{
}

void LayoutGroup::UpdateLayout()
{
	for (const auto& ui : OwningObject.lock()->GetComponentsInChildren<UIComponent>())
	{
		DeregisterAllHandlers(ui->GetGameObject());
		RegisterAllHandlers(ui->GetGameObject());
	}
}