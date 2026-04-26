#pragma once
#include "Data/UI/LayoutPadding.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class DispatchableEvent;
	class UIComponent;


	// TODO: Shouldn't this be a UIComponent?
	class DF2D_API LayoutGroup : public GameComponent
	{
		TYPE_INFO(LayoutGroup, GameComponent);


	private:
		void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	protected:
		float layoutSpacing;

		DeadFrame2D::Data::LayoutPadding layoutPadding;


		virtual void OnChildActiveStateChangedHandler(const ObjectHandle<GameObject>& child, bool activeState) override;

		virtual void OnChildDestroyedHandler(const ObjectHandle<GameObject>& destroyedObj) override;

		virtual void OnChildGameObjectAddedHandler(const ObjectHandle<GameObject>& obj) override;


	public:
		LayoutGroup(float layoutSpacing, DeadFrame2D::Data::LayoutPadding layoutPadding);

		virtual ~LayoutGroup() override = default;


		virtual void Start() override;

		virtual void Update(float deltaTime) override;


		virtual void UpdateLayout();
	};
}