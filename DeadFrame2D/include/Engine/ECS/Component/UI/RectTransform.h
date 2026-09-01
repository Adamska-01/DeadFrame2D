#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Layout/UIAnchor.h"
#include "Data/Components/UI/RectTransform/RectTransformProperties.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"


namespace DF2D::Engine
{
	class Transform;

	// TODO: Should RectTransform be an extension of Transform?
	/**
	 * @brief Places a UI element using anchors, a pivot and a size, the way UI is normally authored.
	 *
	 * This component only translates that model into style properties; the layout engine works out
	 * the actual rectangle. It never positions a child.
	 *
	 * A UI object still carries a Transform, and after each layout pass this writes the resolved
	 * rectangle's centre back into it. That mirror is strictly one way: world position never feeds
	 * back into the anchors, so the two cannot fight.
	 */
	class DF2D_API RectTransform : public UIComponent
	{
		TYPE_INFO(RectTransform, UIComponent);


	private:
		Data::RectTransformProperties properties;

		ComponentHandle<Transform> transform;

		float localRotation = 0.0f;

		Core::Vector2F localScale = Core::Vector2F::One;


		void ApplyPlacement();

		void ApplyTransform();


	protected:
		void OnElementCreated() override;


	public:
		RectTransform();

		virtual ~RectTransform() override = default;


		void LateUpdate(float deltaTime) override;


		/** @brief Pins both anchors to one of the nine standard positions. */
		void SetAnchorPreset(Data::UIAnchor preset);

		void SetAnchorMin(const Core::Vector2F& anchorMin);

		void SetAnchorMax(const Core::Vector2F& anchorMax);

		void SetPivot(const Core::Vector2F& pivot);

		void SetAnchoredPosition(const Core::Vector2F& anchoredPosition);

		void SetSizeDelta(const Core::Vector2F& sizeDelta);

		/** @brief Sets the inset from the anchors on whichever axes are stretched. */
		void SetStretchInset(const Core::Vector2F& inset);

		void SetLocalRotation(float degrees);

		void SetLocalScale(const Core::Vector2F& scale);


		const Data::RectTransformProperties& GetProperties() const;

		Core::Vector2F GetAnchorMin() const;

		Core::Vector2F GetAnchorMax() const;

		Core::Vector2F GetPivot() const;

		Core::Vector2F GetAnchoredPosition() const;

		Core::Vector2F GetSizeDelta() const;

		/** @brief The resolved size in canvas space. Zero until the first layout pass has run. */
		Core::Vector2F GetSize() const;
	};
}