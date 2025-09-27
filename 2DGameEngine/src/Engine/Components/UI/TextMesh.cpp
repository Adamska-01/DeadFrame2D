#include "Core/SubSystems/Systems/TextureManager.h"
#include "Core/SubSystems/Systems/UIManager.h"
#include "Data/Components/UI/TextMeshComponentModel.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/UI/TextMesh.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;


	TextMesh::TextMesh(const TextMeshComponentModel& textMeshConfiguration)
		: text(textMeshConfiguration.text)
	{
		font = UIManager::LoadFont(textMeshConfiguration.fontSource, textMeshConfiguration.fontSize);
	
		initialObjectScale = textMeshConfiguration.textObjectInitialScale;

		centered = false;
		fontSize = 30;

		SetText(text);
		SetTextColor(textMeshConfiguration.textColor);
		SetFontStyle(textMeshConfiguration.fontStyle);
		SetIsCentered(textMeshConfiguration.isCentered);
	}

	void TextMesh::Init()
	{
		UIComponent::Init();

		transform = OwningObject.lock()->GetComponent<Transform>();

		transform->SetLocalScale(initialObjectScale);
	}

	void TextMesh::Start()
	{

	}

	void TextMesh::Update(float deltaTime)
	{
	}

	void TextMesh::Draw()
	{
		auto destRect = GetTextDestRect();

		TextureManager::DrawTextureScreenSpace(textTexture, NULL, &destRect, transform->GetWorldRotation());
	}

	void TextMesh::SetFontSize(unsigned int newFontSize)
	{
		fontSize = newFontSize;
	}

	void TextMesh::SetTextColor(SDL_Color newColor)
	{
		color = newColor;

		textTexture = UIManager::LoadText(font, text, color, centered);
	}

	void TextMesh::SetFontStyle(FontStyle newFontStyle)
	{
		TTF_SetFontStyle(font.get(), newFontStyle);

		textTexture = UIManager::LoadText(font, text, color, centered);
	}

	void TextMesh::SetText(std::string newText)
	{
		text = newText;

		textTexture = UIManager::LoadText(font, text, color, centered);

		auto width = 0, height = 0;
		SDL_QueryTexture(textTexture.get(), NULL, NULL, &width, &height);

		widgetSize.x = static_cast<float>(width);
		widgetSize.y = static_cast<float>(height);
	}

	void TextMesh::SetIsCentered(bool isCentered)
	{
		centered = isCentered;

		textTexture = UIManager::LoadText(font, text, color, centered);
	}

	std::string TextMesh::GetText()
	{
		return text;
	}

	SDL_Rect TextMesh::GetTextDestRect()
	{
		auto currentPosition = transform->GetWorldPosition();
		auto scaledSize = GetWidgetSize();
		auto anchorVector = GetAnchorFromPreset(anchor);

		return SDL_Rect
		{
			static_cast<int>(currentPosition.x - ((scaledSize.x) * anchorVector.x)),
			static_cast<int>(currentPosition.y - ((scaledSize.y) * anchorVector.y)),
			static_cast<int>(scaledSize.x),
			static_cast<int>(scaledSize.y)
		};
	}
}