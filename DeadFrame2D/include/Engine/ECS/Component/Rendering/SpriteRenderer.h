#pragma once
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "Data/Systems/Rendering/RenderFlip.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <optional>
#include <string>


namespace DF2D::Core
{
	class TextureManager;
}


namespace DF2D::Engine
{
	class Transform;


	class DF2D_API SpriteRenderer : public GameComponent
	{
		TYPE_INFO(SpriteRenderer, GameComponent);


	protected:
		Data::TextureID spriteTexture = 0;

		ComponentHandle<Transform> transform;

		Core::Vector2I spriteSize;

		Data::RenderTask renderTask;

		std::string texturePath;

		Core::TextureManager* textureManager = nullptr;

		// Lets a driver component (e.g. SpriteAnimator) render a sub-region of the
		// texture at a specific size/flip instead of the whole loaded sprite.
		std::optional<Core::RectI> overrideSrcRect = std::nullopt;

		std::optional<Core::Vector2I> overrideDrawSize = std::nullopt;

		Data::RenderFlip flipState = Data::RenderFlip::NONE;


	public:
		SpriteRenderer(std::string_view texturePath);

		SpriteRenderer(SpriteRenderer&& other) = default;

		virtual ~SpriteRenderer() override = default;


		virtual void Init() override;

		virtual void Draw() override;


		void LoadSprite(std::string_view texturePath);

		Data::TextureID GetTexture();

		int GetSortOrder() const;

		void SetSortOrder(int sortOrder);


		std::optional<Core::RectI> GetSourceRect() const;

		void SetSourceRect(std::optional<Core::RectI> srcRect);

		Core::Vector2I GetDrawSize() const;

		void SetDrawSize(std::optional<Core::Vector2I> size);

		Data::RenderFlip GetFlipState() const;

		void SetFlipState(Data::RenderFlip flip);
	};
}