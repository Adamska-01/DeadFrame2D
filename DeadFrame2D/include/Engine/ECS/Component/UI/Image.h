#pragma once
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include <SDL.h>
#include <string_view>


namespace DF2D::Core
{
	class TextureManager;
}


namespace DF2D::Engine
{
	class DF2D_API Image : public UIComponent
	{
		TYPE_INFO(Image, UIComponent);


	private:
		Data::TextureID sourceImage = 0;

		SDL_Color color;

		Core::TextureManager* textureManager = nullptr;


	public:
		Image();

		virtual ~Image() override = default;


		virtual void Init() override;

		virtual void Draw() override;


		void LoadSprite(std::string_view texturePath);

		void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	};
}