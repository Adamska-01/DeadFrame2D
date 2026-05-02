#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <memory>


struct SDL_Texture;


namespace DeadFrame2D::Engine
{
	class Transform;


	class DF2D_API SpriteRenderer : public GameComponent
	{
		TYPE_INFO(SpriteRenderer, GameComponent);


	protected:
		std::shared_ptr<SDL_Texture> spriteTexture;

		ComponentHandle<Transform> transform;

		DeadFrame2D::Core::Vector2I spriteSize;

		DeadFrame2D::Data::RenderTask renderTask;


	public:
		SpriteRenderer(std::string_view texturePath);

		SpriteRenderer(SpriteRenderer&& other) = default;

		virtual ~SpriteRenderer() override = default;


		virtual void Init() override;

		virtual void Draw() override;


		void LoadSprite(std::string_view texturePath);

		std::shared_ptr<SDL_Texture> GetTexture();

		int GetSortOrder() const;

		void SetSortOrder(int sortOrder);
	};
}