#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Rendering/ScrollDirection.h"
#include "Engine/Components/Rendering/Sprite.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class DispatchableEvent;


	class ImageScroller : public Sprite
	{
	private:
		DeadFrame2D::Data::ScrollDirection scrollDirection;

		float scrollSpeed;

		float scrollOffset;

		DeadFrame2D::Core::Vector2I renderTargetSize;


		void RenderTargetSizeChangedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	public:
		ImageScroller(std::string_view textureSource, DeadFrame2D::Data::ScrollDirection scrollDirection, float scrollSpeed);

		virtual ~ImageScroller() override;


		virtual void Update(float deltaTime) override;

		virtual void Draw() override;
	};
}