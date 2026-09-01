#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Helpers/UI/StyleValues.h"
#include "Utilities/UI/RectTransformResolver.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	RectTransform::RectTransform()
	{
	}


	void RectTransform::OnElementCreated()
	{
		transform = GetGameObject()->GetTransform();

		ApplyPlacement();
		ApplyTransform();
	}

	void RectTransform::ApplyPlacement()
	{
		for (const auto& resolved : RectTransformResolver::ResolveRectTransform(properties))
		{
			SetStyle(resolved.property, resolved.value);
		}
	}

	void RectTransform::ApplyTransform()
	{
		// The pivot doubles as the transform origin, so rotating a bottom-anchored element turns about
		// the same point its position is measured from.
		SetStyle(UIStyleProperty::TRANSFORM_ORIGIN_X, StyleValues::ToPercent(properties.pivot.x));
		SetStyle(UIStyleProperty::TRANSFORM_ORIGIN_Y, StyleValues::ToPercent(properties.pivot.y));

		if (localRotation == 0.0f && localScale == Vector2F::One)
		{
			ClearStyle(UIStyleProperty::TRANSFORM);

			return;
		}

		auto value = "rotate(" + std::to_string(localRotation) + "deg) scale("
			+ std::to_string(localScale.x) + ", " + std::to_string(localScale.y) + ")";

		SetStyle(UIStyleProperty::TRANSFORM, value);
	}


	void RectTransform::LateUpdate(float deltaTime)
	{
		if (transform == nullptr)
			return;

		// Layout has resolved by now, so mirror the result into the Transform that every GameObject
		// carries. One way only: nothing reads a world position back out into the anchors.
		auto rect = GetScreenRect();

		transform->SetWorldPosition(Vector2F(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.5f));
	}


	void RectTransform::SetAnchorPreset(UIAnchor preset)
	{
		auto anchor = GetAnchorFromPreset(preset);

		properties.anchorMin = anchor;
		properties.anchorMax = anchor;
		properties.pivot = anchor;

		ApplyPlacement();
		ApplyTransform();
	}

	void RectTransform::SetAnchorMin(const Vector2F& anchorMin)
	{
		properties.anchorMin = anchorMin;

		ApplyPlacement();
	}

	void RectTransform::SetAnchorMax(const Vector2F& anchorMax)
	{
		properties.anchorMax = anchorMax;

		ApplyPlacement();
	}

	void RectTransform::SetPivot(const Vector2F& pivot)
	{
		properties.pivot = pivot;

		ApplyPlacement();
		ApplyTransform();
	}

	void RectTransform::SetAnchoredPosition(const Vector2F& anchoredPosition)
	{
		properties.anchoredPosition = anchoredPosition;

		ApplyPlacement();
	}

	void RectTransform::SetSizeDelta(const Vector2F& sizeDelta)
	{
		properties.sizeDelta = sizeDelta;

		ApplyPlacement();
	}

	void RectTransform::SetStretchInset(const Vector2F& inset)
	{
		properties.sizeDelta = inset;

		ApplyPlacement();
	}

	void RectTransform::SetLocalRotation(float degrees)
	{
		localRotation = degrees;

		ApplyTransform();
	}

	void RectTransform::SetLocalScale(const Vector2F& scale)
	{
		localScale = scale;

		ApplyTransform();
	}


	const RectTransformProperties& RectTransform::GetProperties() const
	{
		return properties;
	}

	Vector2F RectTransform::GetAnchorMin() const
	{
		return properties.anchorMin;
	}

	Vector2F RectTransform::GetAnchorMax() const
	{
		return properties.anchorMax;
	}

	Vector2F RectTransform::GetPivot() const
	{
		return properties.pivot;
	}

	Vector2F RectTransform::GetAnchoredPosition() const
	{
		return properties.anchoredPosition;
	}

	Vector2F RectTransform::GetSizeDelta() const
	{
		return properties.sizeDelta;
	}

	Vector2F RectTransform::GetSize() const
	{
		auto rect = GetScreenRect();

		return Vector2F(rect.w, rect.h);
	}
}