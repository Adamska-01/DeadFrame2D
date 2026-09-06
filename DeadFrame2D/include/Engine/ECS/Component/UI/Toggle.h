#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"


namespace DF2D::Engine
{
	/**
	 * @brief A two-state control the player can switch on and off.
	 *
	 * Carries no caption of its own (a toggle is one element and a label is another).
	 */
	class DF2D_API Toggle : public IInteractableUI
	{
		TYPE_INFO(Toggle, IInteractableUI);


	private:
		bool isOn = false;


		void ApplyState();


	protected:
		Data::UIElementType GetElementType() const override;

		void OnElementCreated() override;

		void OnInteraction(Data::UIEventType eventType, const Data::UIEventPayload& payload) override;


	public:
		Toggle();

		virtual ~Toggle() override = default;


		/** @brief Fired whenever the state changes, whether by the player or by SetIsOn. */
		Utilities::MulticastDelegate<bool> OnValueChanged;


		void SetIsOn(bool value);

		bool IsOn() const;
	};
}