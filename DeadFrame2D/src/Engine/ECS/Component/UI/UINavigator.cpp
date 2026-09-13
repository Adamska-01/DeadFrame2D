#include "Core/Context/Systems/Input/Actions/InputActionView.h"
#include "Engine/ECS/Component/UI/UINavigator.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"
#include <algorithm>
#include <cmath>
#include <iostream>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;

	namespace
	{
		constexpr auto NavigationDeadzone = 0.5f;

		constexpr auto MinimumRepeatRate = 0.001f;
	}


	UINavigator::UINavigator()
		: repeatTimer(0.0f),
		repeatDelay(0.4f),
		repeatRate(0.1f)
	{
	}


	void UINavigator::Bind(const std::string& actionName, const std::function<void(const InputActionView&)>& handler)
	{
		// Registering against a name nothing defines reports the failure only in the return value, so
		// without this the navigation would simply never happen and never say why.
		if (playerInput->RegisterAction(actions.actionMap, actionName, GetHandle(), handler) < 0)
		{
			std::cerr << "UINavigator found no action '" << actionName << "' in action map '"
				<< actions.actionMap << "'; that part of UI navigation will not respond." << std::endl;
		}
	}


	void UINavigator::Move(UINavigationDirection direction)
	{
		if (canvas != nullptr)
		{
			canvas->GetContext().Navigate(direction);
		}
	}


	void UINavigator::NavigateHandler(const InputActionView& action)
	{
		auto value = action.ReadValue<Vector2F>();

		if (std::abs(value.x) < NavigationDeadzone && std::abs(value.y) < NavigationDeadzone)
		{
			heldDirection.reset();

			return;
		}

		// The larger axis wins outright: a diagonal push means one of the two, never both.
		//
		// Up reads as a positive y here, while the screen counts y downwards, so the vertical axis is
		// read the way the input describes it rather than the way the layout is measured.
		auto direction = std::abs(value.x) > std::abs(value.y)
			? (value.x > 0.0f ? UINavigationDirection::RIGHT : UINavigationDirection::LEFT)
			: (value.y > 0.0f ? UINavigationDirection::UP : UINavigationDirection::DOWN);

		// A fresh press always moves once, right away. Only what happens afterwards is paced, so
		// pressing a direction repeatedly and quickly registers every press.
		//
		// A new direction counts as a fresh press too, even without the action restarting: pushing from
		// one direction into another should move at once rather than finish the old direction's wait.
		if (!action.IsStarted() && heldDirection == direction)
			return;

		heldDirection = direction;
		repeatTimer = repeatDelay;

		Move(direction);
	}

	void UINavigator::SubmitHandler(const InputActionView& action)
	{
		if (canvas != nullptr && action.IsStarted())
		{
			canvas->GetContext().ActivateFocused();
		}
	}

	void UINavigator::CancelHandler(const InputActionView& action)
	{
		if (action.IsStarted())
		{
			OnCancel.Broadcast();
		}
	}


	void UINavigator::Init()
	{
		canvas = GetGameObject()->GetComponentInParent<Canvas>(true, true);

		Guard::AgainstNull(canvas, NAME_OF(canvas));

		playerInput = GetGameObject()->ServiceContext().sceneManager->FindObjectOfType<PlayerInput>();

		Guard::AgainstNull(playerInput, NAME_OF(playerInput));

		Bind(actions.navigate, [this](const InputActionView& action) { NavigateHandler(action); });
		Bind(actions.submit, [this](const InputActionView& action) { SubmitHandler(action); });
		Bind(actions.cancel, [this](const InputActionView& action) { CancelHandler(action); });

		// Action maps start disabled, so the map this navigator reads from is enabled here rather than
		// left to the game: adding the component is meant to be the whole of making the UI reachable.
		// Enabled alongside whatever gameplay map is already on, not instead of it.
		playerInput->EnableActionMap(actions.actionMap);
	}

	void UINavigator::Start()
	{
		if (firstSelected != nullptr)
		{
			firstSelected->Focus();
		}
	}

	void UINavigator::Update(float deltaTime)
	{
		if (!heldDirection.has_value())
			return;

		repeatTimer -= deltaTime;

		// A loop rather than a single step, so a rate faster than the frame keeps up instead of
		// quietly slowing to one move per frame.
		while (repeatTimer <= 0.0f)
		{
			Move(*heldDirection);

			repeatTimer += repeatRate;
		}
	}


	void UINavigator::SetFirstSelected(const ComponentHandle<IInteractableUI>& widget)
	{
		firstSelected = widget;
	}

	void UINavigator::SetActions(const UINavigationActions& value)
	{
		actions = value;
	}

	const UINavigationActions& UINavigator::GetActions() const
	{
		return actions;
	}

	void UINavigator::SetRepeatTiming(float delaySeconds, float rateSeconds)
	{
		repeatDelay = delaySeconds;

		// A rate of zero would mean an infinite number of moves in one frame, so it is floored at
		// something a frame can actually deliver.
		repeatRate = std::max(rateSeconds, MinimumRepeatRate);
	}
}