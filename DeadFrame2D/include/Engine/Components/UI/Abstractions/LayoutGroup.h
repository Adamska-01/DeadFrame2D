#pragma once
#include "Data/UI/LayoutPadding.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class DispatchableEvent;
	class UIComponent;


	class DF2D_API LayoutGroup : public GameComponent
	{
	private:
		void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

		void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	protected:
		float layoutSpacing;

		DeadFrame2D::Data::LayoutPadding layoutPadding;


		virtual void OnGameObjectActiveStateChangedHandler(GameObject* child, bool activeState) override;


	public:
		LayoutGroup(float layoutSpacing, DeadFrame2D::Data::LayoutPadding layoutPadding);

		virtual ~LayoutGroup() override;


		virtual void Init() override;

		virtual void Start() override;

		virtual void Update(float deltaTime) override;

		virtual void Draw() override;


		virtual void UpdateLayout();
	};
}