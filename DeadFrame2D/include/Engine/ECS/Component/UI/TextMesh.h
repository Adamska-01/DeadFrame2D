#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Text/FontStyle.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <memory>
#include <string>


struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;

namespace DF2D::Data
{
	struct TextMeshComponentModel;
}


namespace DF2D::Engine
{
	class Transform;


	class DF2D_API TextMesh : public UIComponent
	{
		TYPE_INFO(TextMesh, UIComponent);


	private:
		ComponentHandle<Transform> transform;

		Core::Vector2F initialObjectScale;

		std::shared_ptr<SDL_Texture> textTexture;

		std::shared_ptr<TTF_Font> font;

		std::string text;

		SDL_Color color;

		bool centered;

		unsigned int fontSize;


	public:
		TextMesh(const Data::TextMeshComponentModel& textMeshConfiguration);

		virtual ~TextMesh() override = default;


		virtual void Init() override;

		virtual void Draw() override;


		void SetFontSize(unsigned int newFontSize);

		void SetTextColor(SDL_Color newColor);

		void SetFontStyle(Data::FontStyle newFontStyle);

		void SetText(std::string newText);

		void SetIsCentered(bool isCentered);

		std::string GetText();

		SDL_FRect GetTextDestRect();
	};
}