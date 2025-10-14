#pragma once
#include "DF2D_API.h"
#include "Engine/Components/UI/Abstractions/UIComponent.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API Canvas : public UIComponent
	{
		TYPE_INFO(Canvas, UIComponent);


	private:
		// Impossible to implement for now...
		int sortOrder;


	public:
		Canvas();


		virtual void Init() override;

		virtual void Start() override;

		virtual void Update(float deltaTime) override;

		virtual void Draw() override;


		int GetSortOrder() const;

		void SetSortOrder(int value);
	};
}