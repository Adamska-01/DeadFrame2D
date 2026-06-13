#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/Rendering/ScrollDirection.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/Rendering/SpriteRenderer.h"
#include <memory>


namespace DF2D::Engine
{
	class DispatchableEvent;


	class DF2D_API ImageScroller : public SpriteRenderer
	{
		TYPE_INFO(ImageScroller, SpriteRenderer);


	private:
		Data::ScrollDirection scrollDirection;

		float scrollSpeed;

		float scrollOffset;

		Core::Vector2I renderTargetSize;


		void RenderTargetSizeChangedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


	public:
		ImageScroller(std::string_view textureSource, Data::ScrollDirection scrollDirection, float scrollSpeed);

		virtual ~ImageScroller() override;


		virtual void Init() override;

		virtual void Update(float deltaTime) override;

		virtual void Draw() override;
	};
}