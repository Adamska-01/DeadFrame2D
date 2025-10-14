#pragma once
#include "Core/Math/Vector2.h"
#include "Data/UI/FontStyle.h"
#include "DF2D_API.h"
#include "Engine/Components/UI/Abstractions/UIComponent.h"
#include "Engine/Entity/ComponentHandle.h"
#include <memory>
#include <string>


struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;

namespace DeadFrame2D::Data
{
	struct TextMeshComponentModel;
}


namespace DeadFrame2D::Engine
{
	class Transform;


	class DF2D_API TextMesh : public UIComponent
	{
		TYPE_INFO(TextMesh, UIComponent);


	private:
		ComponentHandle<Transform> transform;

		DeadFrame2D::Core::Vector2F initialObjectScale;

		std::shared_ptr<SDL_Texture> textTexture;

		std::shared_ptr<TTF_Font> font;

		std::string text;

		SDL_Color color;

		bool centered;

		unsigned int fontSize;


	public:
		TextMesh(const DeadFrame2D::Data::TextMeshComponentModel& textMeshConfiguration);

		virtual ~TextMesh() override = default;


		virtual void Init() override;

		virtual void Start() override;

		virtual void Update(float deltaTime) override;

		virtual void Draw() override;


		void SetFontSize(unsigned int newFontSize);

		void SetTextColor(SDL_Color newColor);

		void SetFontStyle(DeadFrame2D::Data::FontStyle newFontStyle);

		void SetText(std::string newText);

		void SetIsCentered(bool isCentered);

		std::string GetText();

		SDL_Rect GetTextDestRect();
	};
}