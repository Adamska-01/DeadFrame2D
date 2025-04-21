#include "Components/Transform.h"
#include "GameObject.h"
#include "Math/MathConstants.h"


Transform::Transform(Vector2F position, Vector2F scale, float angle)
	: localPosition(position), localScale(scale), localRotation(angle)
{
}

void Transform::MarkDirty()
{
	if (isDirty)
		return;

	isDirty = true;

	// Propagate to children
	if (OwningObject)
	{
		const auto& children = OwningObject->GetChildren();
		for (const auto& weakChild : children)
		{
			auto child = weakChild.lock();
			if (!child)
				continue;

			auto childTransform = child->GetComponent<Transform>();
			if (childTransform)
			{
				childTransform->MarkDirty();
			}
		}
	}
}

void Transform::RecalculateWorldTransform() const
{
	if (!OwningObject || !OwningObject->GetParent())
	{
		position = localPosition;
		scale = localScale;
		rotation = localRotation;
	}
	else
	{
		auto parentTransform = OwningObject->GetParent()->GetComponent<Transform>();

		if (parentTransform)
		{
			auto rotated = localPosition.Rotated(parentTransform->GetWorldRotation());
			auto scaled = Vector2F
			{
				rotated.x * parentTransform->GetWorldScale().x,
				rotated.y * parentTransform->GetWorldScale().y
			};

			position = parentTransform->GetWorldPosition() + scaled;
			scale = parentTransform->GetWorldScale() * localScale;
			rotation = parentTransform->GetWorldRotation() + localRotation;
		}
		else
		{
			position = localPosition;
			scale = localScale;
			rotation = localRotation;
		}
	}

	isDirty = false;
}

void Transform::TranslateX(float x)
{
	localPosition.x += x;

	MarkDirty();
}

void Transform::TranslateY(float y)
{
	localPosition.y += y;
	
	MarkDirty();
}

void Transform::Translate(Vector2F v)
{
	localPosition += v;

	MarkDirty();
}

void Transform::ScaleX(float x)
{
	localScale.x = x;

	MarkDirty();
}

void Transform::ScaleY(float y)
{
	localScale.y = y;

	MarkDirty();
}

void Transform::Scale(Vector2F v)
{
	localScale = v;

	MarkDirty();
}

void Transform::RotateByRadians(float radian)
{
	localRotation += radian * (180.0f / MathConstants::PI);

	MarkDirty();
}

void Transform::RotateByDegrees(float degreeAngle)
{
	localRotation += degreeAngle;

	MarkDirty();
}

void Transform::Init()
{
}

void Transform::Update(float dt)
{
	startFramePosition = position;
}

void Transform::Draw()
{
}

Vector2F Transform::GetStartFramePosition() const
{
	return startFramePosition;
}

Vector2F Transform::GetLocalPosition() const
{
	return localPosition;
}

Vector2F Transform::GetWorldPosition() const
{
	if (isDirty)
		RecalculateWorldTransform();

	return position;
}

Vector2F Transform::GetLocalScale() const
{
	return localScale;
}

Vector2F Transform::GetWorldScale() const
{
	if (isDirty)
		RecalculateWorldTransform();

	return scale;
}

float Transform::GetLocalRotation() const
{
	return localRotation;
}

float Transform::GetWorldRotation() const
{
	if (isDirty)
		RecalculateWorldTransform();

	return rotation;
}

Vector2F Transform::GetForward() const
{
	auto radians = GetWorldRotation() * (MathConstants::PI / 180.0f);

	return Vector2F(std::cos(radians), std::sin(radians));
}

void Transform::SetLocalPosition(const Vector2F& pos)
{
	localPosition = pos;
	
	MarkDirty();
}

void Transform::SetWorldPosition(const Vector2F& worldPos)
{
	auto parent = OwningObject ? OwningObject->GetParent() : nullptr;
	auto parentTransform = parent ? parent->GetComponent<Transform>() : nullptr;

	if (parentTransform == nullptr)
	{
		localPosition = worldPos;
	}
	else
	{
		auto invScale = Vector2F
		{
			1.0f / parentTransform->GetWorldScale().x,
			1.0f / parentTransform->GetWorldScale().y
		};

		auto delta = worldPos - parentTransform->GetWorldPosition();

		Vector2F unscaled = Vector2F
		{
			delta.x * invScale.x,
			delta.y * invScale.y
		};

		localPosition = unscaled.Rotated(-parentTransform->GetWorldRotation());
	}

	MarkDirty();
}

void Transform::SetLocalScale(const Vector2F& scale)
{
	localScale = scale;
	
	MarkDirty();
}

void Transform::SetWorldScale(const Vector2F& worldScale)
{
	auto parent = OwningObject ? OwningObject->GetParent() : nullptr;
	auto parentTransform = parent ? parent->GetComponent<Transform>() : nullptr;

	if (!parentTransform)
	{
		localScale = worldScale;
	}
	else
	{
		localScale = Vector2F
		{
			worldScale.x / parentTransform->GetWorldScale().x,
			worldScale.y / parentTransform->GetWorldScale().y
		};
	}

	MarkDirty();
}

void Transform::SetLocalRotation(float rotation)
{
	localRotation = rotation; 
	
	MarkDirty();
}

void Transform::SetWorldRotation(float worldRotation)
{
	auto parent = OwningObject ? OwningObject->GetParent() : nullptr;
	auto parentTransform = parent ? parent->GetComponent<Transform>() : nullptr;

	localRotation = parentTransform
		? worldRotation - parentTransform->GetWorldRotation()
		: worldRotation;

	MarkDirty();
}