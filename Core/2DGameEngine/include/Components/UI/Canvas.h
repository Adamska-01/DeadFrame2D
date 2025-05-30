#pragma once
#include "Components/UI/Abstractions/UIComponent.h"


class Canvas : public UIComponent
{
private:
	// Impossible to implement for now...
	int sortOrder;


public:
	Canvas();


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	int SetSortOrder() const;

	void SetSortOrder(int value);
};