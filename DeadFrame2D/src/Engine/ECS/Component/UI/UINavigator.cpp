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


	bool UINavigator::IsSuppressed(UINavigationDirection direction) const
	{
		auto isHorizontal = direction == UINavigationDirection::LEFT || direction == UINavigationDirection::RIGHT;

		return (isHorizontal && horizontalSuppressed) || (!isHorizontal && verticalSuppressed);
	}

	void UINavigator::NavigateHandler(const InputActionView& action)
	{
		auto value = action.ReadValue<Vector2F>();

		if (std::abs(value.x) < NavigationDeadzone && std::abs(value.y) < NavigationDeadzone)
		{
			heldDirection.reset();

			return;
		}

		// Use the dominant axis, so a diagonal input moves in one direction only.
		// Input uses positive Y for up, while the layout uses positive Y for down.
		auto direction = std::abs(value.x) > std::abs(value.y)
			? (value.x > 0.0f ? UINavigationDirection::RIGHT : UINavigationDirection::LEFT)
			: (value.y > 0.0f ? UINavigationDirection::UP : UINavigationDirection::DOWN);

		// A fresh press moves immediately. Holding the same direction is handled by the repeat timer.
		// Changing direction also counts as a fresh press.
		if (!action.IsStarted() && heldDirection == direction)
			return;

		// Keep track of the held direction even when navigation is suppressed.
		heldDirection = direction;
		repeatTimer = repeatDelay;

		if (!IsSuppressed(direction))
		{
			Move(direction);
		}
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
			if (!IsSuppressed(*heldDirection))
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

	void UINavigator::SetHorizontalNavigationSuppressed(bool suppressed)
	{
		horizontalSuppressed = suppressed;
	}

	void UINavigator::SetVerticalNavigationSuppressed(bool suppressed)
	{
		verticalSuppressed = suppressed;
	}
}