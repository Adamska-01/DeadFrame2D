#pragma once
#include "Data/Rendering/ScrollDirection.h"
#include <Components/Rendering/Sprite.h>
#include <Math/Vector2.h>
#include <memory>


class DispatchableEvent;


class ImageScroller : public Sprite
{
private:
	ScrollDirection scrollDirection;

	float scrollSpeed;

	float scrollOffset;

	Vector2I renderTargetSize;


	void RenderTargetSizeChangedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


public:
	ImageScroller(std::string_view textureSource, ScrollDirection scrollDirection, float scrollSpeed);

	virtual ~ImageScroller() override;


	virtual void Update(float deltaTime) override;

	virtual void Draw() override;
};