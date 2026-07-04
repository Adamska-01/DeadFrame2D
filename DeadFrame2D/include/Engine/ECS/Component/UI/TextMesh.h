#pragma once
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Text/FontStyle.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <string>


namespace DF2D::Core
{
	class UIManager;
}

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
		Core::UIManager* uiManager;

		ComponentHandle<Transform> transform;

		Core::Vector2F initialObjectScale;

		Data::TextureID textTexture = 0;

		std::string fontSource;

		int fontSize = 30;

		std::string text;

		Core::Color color;

		Data::FontStyle fontStyle = Data::FontStyle::NORMAL;

		bool centered;


		void RebuildTextTexture();


	public:
		TextMesh(const Data::TextMeshComponentModel& textMeshConfiguration);

		virtual ~TextMesh() override = default;


		virtual void Init() override;

		virtual void Draw() override;


		void SetFontSize(int newFontSize);

		void SetTextColor(Core::Color newColor);

		void SetFontStyle(Data::FontStyle newFontStyle);

		void SetText(std::string newText);

		void SetIsCentered(bool isCentered);

		std::string GetText();

		Core::RectF GetTextDestRect();
	};
}