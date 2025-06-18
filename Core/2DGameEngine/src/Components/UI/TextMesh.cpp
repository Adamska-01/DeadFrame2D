#include "Components/Transform.h"
#include "Components/UI/TextMesh.h"
#include "GameObject.h"
#include "Models/Components/UI/TextMeshComponentModel.h"
#include "SubSystems/TextureManager.h"
#include "SubSystems/UIManager.h"


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
	auto destRect = GetTextBoundingBox();

	TextureManager::DrawTexture(textTexture, NULL, &destRect, transform->GetWorldRotation());
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

	widgetSize.x = static_cast<int>(width);
	widgetSize.y = static_cast<int>(height);
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

SDL_Rect TextMesh::GetTextBoundingBox()
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