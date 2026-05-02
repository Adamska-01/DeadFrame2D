#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Layout/UIAnchor.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"


namespace DeadFrame2D::Engine
{
	class Transform;
	class Canvas;


	/**
	 * @brief Represents a generic UI Component.
	 */
	class DF2D_API UIComponent : public GameComponent
	{
		TYPE_INFO(UIComponent, GameComponent);


	protected:
		ComponentHandle<Canvas> parentCanvas;

		DeadFrame2D::Data::UIAnchor anchor;

		DeadFrame2D::Core::Vector2F widgetSize;

		ComponentHandle<Transform> transform;

		DeadFrame2D::Data::RenderTask renderTask;


		void OnParentGameObjectChangedHandler(const ObjectHandle<GameObject>& obj) override;


	public:
		UIComponent();

		virtual ~UIComponent() override = default;


		void Init() override;

		void Update(float deltaTime) override;


		void SetAnchor(DeadFrame2D::Data::UIAnchor newAnchor);

		void SetWidgetSize(DeadFrame2D::Core::Vector2F newWidgetSize);

		DeadFrame2D::Core::Vector2F GetWidgetSize() const;
	};
}