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
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &LayoutGroup::GameObjectCreatedHandler));
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &LayoutGroup::GameObjectDestroyedHandler));
}

LayoutGroup::~LayoutGroup()
{
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectCreatedEvent)), EventHelpers::BindFunction(this, &LayoutGroup::GameObjectCreatedHandler));
	EventDispatcher::RegisterEventHandler(std::type_index(typeid(GameObjectDestroyedEvent)), EventHelpers::BindFunction(this, &LayoutGroup::GameObjectDestroyedHandler));
}

void LayoutGroup::GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectCreatedEvent>(dispatchableEvent);

	if (gameObjEvent == nullptr || gameObjEvent->gameObjectCreated == nullptr)
		return;

	auto& target = gameObjEvent->gameObjectCreated;

	if (!target->IsChildOf(OwningObject))
		return;

	target->OnActiveStateChanged -= EventHelpers::BindFunction(this, &LayoutGroup::OnActiveStateChangeHandler);
	target->OnActiveStateChanged += EventHelpers::BindFunction(this, &LayoutGroup::OnActiveStateChangeHandler);

	MarkDirty();
}

void LayoutGroup::GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
{
	auto gameObjEvent = DispatchableEvent::SafeCast<GameObjectDestroyedEvent>(dispatchableEvent);

	if (!gameObjEvent || gameObjEvent->gameObjectDestroyed == nullptr)
		return;

	auto target = gameObjEvent->gameObjectDestroyed;

	if (!target->IsChildOf(OwningObject))
		return;

	MarkDirty();
}

void LayoutGroup::OnActiveStateChangeHandler(GameObject* child, bool activeState)
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
	for (const auto& ui : OwningObject->GetComponentsInChildren<UIComponent>())
	{
		ui->GetGameObject()->OnActiveStateChanged -= EventHelpers::BindFunction(this, &LayoutGroup::OnActiveStateChangeHandler);
		ui->GetGameObject()->OnActiveStateChanged += EventHelpers::BindFunction(this, &LayoutGroup::OnActiveStateChangeHandler);
	}
}