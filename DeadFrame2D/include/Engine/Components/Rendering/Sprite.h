#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include <memory>
#include <string>


struct SDL_Texture;


namespace DeadFrame2D::Engine
{
	class Transform;


	class DF2D_API Sprite : public GameComponent
	{
		TYPE_INFO(Sprite, GameComponent);


	protected:
		std::shared_ptr<SDL_Texture> spriteTexture;

		Transform* transform;

		DeadFrame2D::Core::Vector2I spriteSize;


	public:
		Sprite(std::string_view texturePath);

		Sprite(Sprite&& other) = default;

		virtual ~Sprite() override = default;


		virtual void Init() override;

		virtual void Start() override;

		virtual void Update(float deltaTime) override;

		virtual void Draw() override;


		void LoadSprite(std::string_view texturePath);

		std::shared_ptr<SDL_Texture> GetTexture();
	};
}