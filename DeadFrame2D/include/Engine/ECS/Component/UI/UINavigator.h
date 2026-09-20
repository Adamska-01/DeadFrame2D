#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Navigation/UINavigationActions.h"
#include "Data/Systems/UI/UINavigationDirection.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/Input/PlayerInput.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>
#include <optional>
#include <string>


namespace DF2D::Engine
{
	/**
	 * @brief Handles navigation between UI widgets on a canvas.
	 *
	 * Sits beside the Canvas, and needs a PlayerInput in the scene.
	 */
	class DF2D_API UINavigator : public GameComponent
	{
		TYPE_INFO(UINavigator, GameComponent);


	private:
		ComponentHandle<Canvas> canvas;

		ComponentHandle<PlayerInput> playerInput;

		ComponentHandle<IInteractableUI> firstSelected;

		Data::UINavigationActions actions;

		/** @brief The direction currently being held, if any. Empty means nothing is pushed. */
		std::optional<Data::UINavigationDirection> heldDirection;

		/** @brief Seconds left before the held direction moves focus again. */
		float repeatTimer;

		/** @brief Seconds a direction must be held before it starts repeating. */
		float repeatDelay;

		/** @brief Seconds between repeats once one has started. */
		float repeatRate;

		/** @brief While true, LEFT/RIGHT presses are swallowed instead of moving focus. */
		bool horizontalSuppressed = false;

		/** @brief While true, UP/DOWN presses are swallowed instead of moving focus. */
		bool verticalSuppressed = false;


		/** @brief Registers one action and says so plainly if the name does not resolve to anything. */
		void Bind(const std::string& actionName, const std::function<void(const Core::InputActionView&)>& handler);


		void NavigateHandler(const Core::InputActionView& action);

		void SubmitHandler(const Core::InputActionView& action);

		void CancelHandler(const Core::InputActionView& action);

		/** @brief Moves focus one step, if there is a canvas to move it on. */
		void Move(Data::UINavigationDirection direction);

		/** @brief Whether the axis `direction` falls on is currently gated off. */
		bool IsSuppressed(Data::UINavigationDirection direction) const;


	public:
		UINavigator();

		virtual ~UINavigator() override = default;


		void Init() override;

		void Start() override;

		void Update(float deltaTime) override;


		/** @brief Fired when the player asks to back out. What that means is the game's to decide. */
		Utilities::MultiCastVoid OnCancel;


		/** @brief The widget focus starts on when the scene opens. */
		void SetFirstSelected(const ComponentHandle<IInteractableUI>& widget);

		/**
		 * @brief Points this navigator at different actions than the engine's defaults.
		 *
		 * Set before the component initialises, since that is when the actions are subscribed to. A game
		 * with its own name for backing out points Cancel at it rather than binding the same button twice.
		 */
		void SetActions(const Data::UINavigationActions& value);

		const Data::UINavigationActions& GetActions() const;

		/**
		 * @brief Sets how long a held direction waits before repeating, and how fast it repeats after.
		 *
		 * A press always moves focus once immediately; these only govern what holding does next.
		 */
		void SetRepeatTiming(float delaySeconds, float rateSeconds);

		/**
		 * @brief Prevents one navigation axis from moving focus.
		 *
		 * Useful when a focused widget uses that axis for its own input, such as a Slider using
		 * LEFT/RIGHT to change its value. Changing the suppression state also clears any held direction.
		 */
		void SetHorizontalNavigationSuppressed(bool suppressed);

		/**
		 * @brief Prevents vertical navigation from moving focus.
		 *
		 * Useful when a focused widget uses UP/DOWN for its own input. Changing the suppression state also
		 * clears any held direction.
		 */
		void SetVerticalNavigationSuppressed(bool suppressed);
	};
}