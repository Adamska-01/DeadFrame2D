#pragma once
#include "Data/Components/UI/Slider/SliderColors.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Image.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"


namespace DF2D::Engine
{
	/**
	 * @brief A draggable slider producing a value between a minimum and maximum.
	 *
	 * Fill and handle elements are independent and optional. A slider can therefore use either,
	 * both, or neither.
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

		/** @brief Prevents state changes triggered by the slider itself from being applied twice. */
		bool applyingState = false;

		/** @brief Stretched from the start of the slider to the value. Optional. */
		ComponentHandle<RectTransform> fillRect;

		/** @brief Pinned to the value, and kept inside the slider at both ends. Optional. */
		ComponentHandle<RectTransform> handleRect;

		ComponentHandle<Image> backgroundGraphic;

		ComponentHandle<Image> fillGraphic;

		ComponentHandle<Image> handleGraphic;

		Data::SliderColors colors;

		/** @brief Interpolated visual position, allowing the handle to ease towards the value. */
		float displayedFraction = 0.0f;

		Data::SliderStateColors displayedColors;


		void ApplyRange();

		void ApplyValue();

		void ApplyOrientation();


		/** @brief Converts the current value to a normalized 0..1 position. */
		float ValueFraction() const;

		/** @brief Returns the colours for the slider's current interaction state. */
		const Data::SliderStateColors& TargetColors() const;

		/** @brief Updates the optional visual elements and their colours. */
		void ApplyVisuals();

		/** @brief Immediately applies the current value and state without interpolation. */
		void SnapVisuals();


	protected:
		Data::UIElementType GetElementType() const override;

		void OnInteractableCreated() override;

		void OnInteraction(Data::UIEventType eventType, const Data::UIEventPayload& payload) override;

		void OnActivationStateChanged(bool isActivated) override;


	public:
		Slider();

		virtual ~Slider() override = default;


		Utilities::MulticastDelegate<float> OnValueChanged;


		void Update(float deltaTime) override;


		bool HasActivationState() const override;

		Data::UINavigationResponse ResolveNavigation(Data::UINavigationDirection direction) const override;


		/** @brief Sets the minimum and maximum values. Safe before initialisation. */
		void SetRange(float min, float max);

		/** @brief Sets the step size. Zero disables stepping. */
		void SetStep(float newStep);

		void SetValue(float newValue);

		/** @brief Switches between horizontal and vertical layout. */
		void SetVertical(bool isVertical);

		/**
		 * @brief Sets the optional fill RectTransform.
		 *
		 * Its anchors are driven from the slider start to the current value. Pass an empty handle to disable it.
		 */
		void SetFillRect(const ComponentHandle<RectTransform>& value);

		/** @brief Sets the optional handle RectTransform, positioned at the current value. */
		void SetHandleRect(const ComponentHandle<RectTransform>& value);

		/** @brief Sets the optional background graphic. */
		void SetBackgroundImage(const ComponentHandle<Image>& value);

		/** @brief Sets the optional fill graphic. */
		void SetFillImage(const ComponentHandle<Image>& value);

		/** @brief Sets the optional handle graphic. */
		void SetHandleImage(const ComponentHandle<Image>& value);

		/** @brief Sets the colours used for the slider's interaction states. */
		void SetColors(const Data::SliderColors& value);

		const Data::SliderColors& GetColors() const;

		float GetValue() const;

		float GetMinimum() const;

		float GetMaximum() const;

		bool IsVertical() const;
	};
}