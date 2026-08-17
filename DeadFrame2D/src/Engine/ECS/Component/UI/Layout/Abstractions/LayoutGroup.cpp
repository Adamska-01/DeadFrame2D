#include "Engine/ECS/Component/UI/Layout/Abstractions/LayoutGroup.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	LayoutGroup::LayoutGroup(float layoutSpacing, LayoutPadding layoutPadding)
		: layoutSpacing(layoutSpacing), layoutPadding(layoutPadding)
	{
	}

	void LayoutGroup::OnChildActiveStateChangedHandler(const ObjectHandle<GameObject>& child, bool activeState)
	{
		MarkDirty();
	}

	void LayoutGroup::OnChildGameObjectAddedHandler(const ObjectHandle<GameObject>& obj)
	{
		MarkDirty();
	}

	void LayoutGroup::OnChildDestroyedHandler(const ObjectHandle<GameObject>& destroyedObj)
	{
		MarkDirty();
	}

	void LayoutGroup::Start()
	{
		UpdateLayout();
	}

	void LayoutGroup::Update(float deltaTime)
	{
		if (isDirty)
		{
			UpdateLayout();

			isDirty = false;
		}
	}

	void LayoutGroup::UpdateLayout()
	{
	}
}