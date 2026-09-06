#include "Core/Context/Systems/UI/Context/UIContext.h"
#include "Core/Context/Systems/UI/UIManager.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Engine;


	UIContext::UIContext(UIManager* manager, UIContextID id)
		: manager(manager),
		id(id)
	{
	}


	bool UIContext::operator==(const UIContext& other) const
	{
		return manager == other.manager && id == other.id;
	}

	bool UIContext::operator!=(const UIContext& other) const
	{
		return !(*this == other);
	}


	bool UIContext::IsValid() const
	{
		return manager != nullptr && id != 0;
	}

	UIContextID UIContext::Id() const
	{
		return id;
	}


	void UIContext::SetSize(Vector2I size)
	{
		if (IsValid())
		{
			manager->SetContextSize(id, size);
		}
	}

	bool UIContext::LoadStyleSheet(const std::string& path)
	{
		return IsValid() && manager->LoadStyleSheet(id, path);
	}

	UIElement UIContext::GetRootElement() const
	{
		return IsValid() ? UIElement(manager, manager->GetRootElement(id)) : UIElement();
	}

	GeometryDrawList UIContext::Render()
	{
		return IsValid() ? manager->RenderContext(id) : GeometryDrawList{};
	}

	void UIContext::Destroy()
	{
		if (!IsValid())
			return;

		manager->DestroyContext(id);

		// Cleared rather than left dangling: a destroyed surface's handle must be inert, not a way to
		// reach whatever the backend hands out for that id next.
		manager = nullptr;
		id = 0;
	}


	void UIContext::DeclareElementType(const ObjectHandle<GameObject>& owningObject, UIElementType type)
	{
		if (IsValid())
		{
			manager->DeclareElementType(id, owningObject, type);
		}
	}

	UIElement UIContext::AcquireElement(const ObjectHandle<GameObject>& owningObject)
	{
		return IsValid() ? UIElement(manager, manager->AcquireElement(id, owningObject)) : UIElement();
	}

	void UIContext::ReleaseElement(const ObjectHandle<GameObject>& owningObject)
	{
		if (IsValid())
		{
			manager->ReleaseElement(owningObject);
		}
	}
}