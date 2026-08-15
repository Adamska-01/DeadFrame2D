#include "Constants/MathConstants.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Constants;


	Transform::Transform(Vector2F position, Vector2F scale, float angle)
		: localPosition(position), localScale(scale), localRotation(angle)
	{
	}

	void Transform::RecalculateWorldTransform() const
	{
		auto gameObject = GetGameObject();

		if (gameObject == nullptr || gameObject->GetParent() == nullptr)
		{
			position = localPosition;
			scale = localScale;
			rotation = localRotation;
		}
		else
		{
			auto parentTransform = gameObject->GetParent()->GetTransform();

			if (parentTransform != nullptr)
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

	void Transform::MarkDirty()
	{
		if (isDirty)
			return;

		isDirty = true;

		auto gameObject = GetGameObject();

		// Propagate to children
		if (gameObject == nullptr)
			return;

		const auto& children = gameObject->GetChildren();
		for (const auto& child : children)
		{
			if (!child)
				continue;

			auto childTransform = child->GetComponent<Transform>();

			if (childTransform == nullptr)
				continue;

			childTransform->MarkDirty();
		}
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
		localRotation += radian * (180.0f / MathConstants::PI_f);

		MarkDirty();
	}

	void Transform::RotateByDegrees(float degreeAngle)
	{
		localRotation += degreeAngle;

		MarkDirty();
	}

	void Transform::Update(float dt)
	{
		startFramePosition = position;
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
		auto radians = GetWorldRotation() * (MathConstants::PI_f / 180.0f);

		return Vector2F(cosf(radians), sinf(radians));
	}

	void Transform::SetLocalPosition(const Vector2F& pos)
	{
		localPosition = pos;
	
		MarkDirty();
	}

	void Transform::SetWorldPosition(const Vector2F& worldPos)
	{
		auto gameObject = GetGameObject();
		auto parent = gameObject != nullptr ? gameObject->GetParent() : ObjectHandle<GameObject>();
		auto parentTransform = parent != nullptr ? parent->GetTransform() : ComponentHandle<Transform>();

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
		auto gameObject = GetGameObject();
		
		auto parent = gameObject != nullptr ? gameObject->GetParent() : ObjectHandle<GameObject>();
		auto parentTransform = parent != nullptr ? parent->GetTransform() : ComponentHandle<Transform>();

		if (parentTransform == nullptr)
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
		auto gameObject = GetGameObject();
		
		auto parent = gameObject != nullptr ? gameObject->GetParent() : ObjectHandle<GameObject>();
		auto parentTransform = parent != nullptr ? parent->GetTransform() : ComponentHandle<Transform>();

		localRotation = parentTransform
			? worldRotation - parentTransform->GetWorldRotation()
			: worldRotation;

		MarkDirty();
	}
}