#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"


namespace DF2D::Engine
{
	/**
	 * @brief A draggable control producing a number between a minimum and a maximum.
	 */
	class DF2D_API Slider : public IInteractableUI
	{
		TYPE_INFO(Slider, IInteractableUI);


	private:
		float minimum = 0.0f;

		float maximum = 1.0f;

		float step = 0.0f;

		float value = 0.0f;

		bool vertical = false;


		void ApplyRange();

		void ApplyValue();

		void ApplyOrientation();


	protected:
		Data::UIElementType GetElementType() const override;

		void OnElementCreated() override;

		void OnInteraction(Data::UIEventType eventType, const Data::UIEventPayload& payload) override;


	public:
		Slider();

		virtual ~Slider() override = default;


		/**
		 * @brief Fired whenever the value changes, whether by the player or by SetValue.
		 *
		 * A value the backend snaps to the step is reported again with the snapped number, so what this
		 * delegate carries is always a value the slider can actually hold.
		 */
		Utilities::MulticastDelegate<float> OnValueChanged;


		/** @brief Sets the bounds. Safe to call before the component is initialised. */
		void SetRange(float min, float max);

		/** @brief Sets the distance between the values the slider can settle on. Zero means continuous. */
		void SetStep(float newStep);

		void SetValue(float newValue);

		/** @brief Lays the slider out down the screen instead of across it. */
		void SetVertical(bool isVertical);


		float GetValue() const;

		float GetMinimum() const;

		float GetMaximum() const;

		bool IsVertical() const;
	};
}