#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Rendering/Pipeline/RenderTask.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/ComponentHandle.h"
#include <memory>


struct SDL_Texture;


namespace DeadFrame2D::Engine
{
	class Transform;


	// TODO: Rename this into SpriteRenderer
	class DF2D_API Sprite : public GameComponent
	{
		TYPE_INFO(Sprite, GameComponent);


	protected:
		std::shared_ptr<SDL_Texture> spriteTexture;

		ComponentHandle<Transform> transform;

		DeadFrame2D::Core::Vector2I spriteSize;

		DeadFrame2D::Data::RenderTask renderTask;


	public:
		Sprite(std::string_view texturePath);

		Sprite(Sprite&& other) = default;

		virtual ~Sprite() override = default;


		virtual void Init() override;

		virtual void Draw() override;


		void LoadSprite(std::string_view texturePath);

		std::shared_ptr<SDL_Texture> GetTexture();

		int GetSortOrder() const;

		void SetSortOrder(int sortOrder);
	};
}