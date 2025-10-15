#pragma once
#include "Core/Math/Vector2.h"
#include "Data/UI/UIAnchor.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/ComponentHandle.h"


namespace DeadFrame2D::Engine
{
	class Transform;


	/**
	 * @brief Represents a generic UI Component.
	 */
	class DF2D_API UIComponent : public GameComponent
	{
		TYPE_INFO(UIComponent, GameComponent);


	protected:
		DeadFrame2D::Data::UIAnchor anchor;

		DeadFrame2D::Core::Vector2F widgetSize;

		ComponentHandle<Transform> transform;


	public:
		UIComponent();

		virtual ~UIComponent() override = default;


		virtual void Init() override;


		void SetAnchor(DeadFrame2D::Data::UIAnchor newAnchor);

		void SetWidgetSize(DeadFrame2D::Core::Vector2F newWidgetSize);

		DeadFrame2D::Core::Vector2F GetWidgetSize() const;
	};
}