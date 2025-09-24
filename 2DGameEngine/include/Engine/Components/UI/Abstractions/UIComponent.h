#pragma once
#include "Engine/Components/GameComponent.h"
#include "Core/Math/Vector2.h"
#include <Data/UI/UIAnchor.h>


namespace DeadFrame2D::Engine
{
	class Transform;


	/**
	 * @brief Represents a generic UI Component.
	 */
	class UIComponent : public GameComponent
	{
	protected:
		DeadFrame2D::Data::UIAnchor anchor;

		DeadFrame2D::Core::Vector2F widgetSize;

		Transform* transform;


	public:
		UIComponent();

		virtual ~UIComponent() override = default;


		virtual void Init() override;

		virtual void Update(float deltaTime) override = 0;

		virtual void Draw() override = 0;


		void SetAnchor(DeadFrame2D::Data::UIAnchor newAnchor);

		void SetWidgetSize(DeadFrame2D::Core::Vector2F newWidgetSize);

		DeadFrame2D::Core::Vector2F GetWidgetSize() const;
	};
}