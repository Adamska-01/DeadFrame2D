#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
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
	};
}