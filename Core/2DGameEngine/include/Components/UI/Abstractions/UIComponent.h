#pragma once
#include "Components/GameComponent.h"
#include "Math/Vector2.h"


/**
 * @brief Represents a generic UI Component.
 */
class UIComponent : public GameComponent
{
protected:
	Vector2F widgetSize;


public:
	UIComponent() = default;

	virtual ~UIComponent() override = default;


	virtual void Init() override = 0;
	
	virtual void Update(float deltaTime) override = 0;
	
	virtual void Draw() override = 0;


	Vector2F GetWidgetSize() const;
};