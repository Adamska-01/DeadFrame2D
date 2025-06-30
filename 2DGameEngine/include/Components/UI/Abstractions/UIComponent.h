#pragma once
#include "Components/GameComponent.h"
#include "Math/Vector2.h"
#include <Data/UI/UIAnchor.h>


class Transform;


/**
 * @brief Represents a generic UI Component.
 */
class UIComponent : public GameComponent
{
protected:
	UIAnchor anchor;

	Vector2F widgetSize;

	Transform* transform;


public:
	UIComponent();

	virtual ~UIComponent() override = default;


	virtual void Init() override;
	
	virtual void Update(float deltaTime) override = 0;
	
	virtual void Draw() override = 0;


	void SetAnchor(UIAnchor newAnchor);

	void SetWidgetSize(Vector2F newWidgetSize);

	Vector2F GetWidgetSize() const;
};