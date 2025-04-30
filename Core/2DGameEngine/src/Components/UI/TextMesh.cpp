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

	SetText(text, textMeshConfiguration.linesNumber);
	SetTextColor(textMeshConfiguration.textColor);
	SetFontStyle(textMeshConfiguration.fontStyle);
}

void TextMesh::Init()
{
	transform = OwningObject->GetComponent<Transform>();

	transform->SetLocalScale(initialObjectScale);
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

	textTexture = UIManager::LoadText(font, text, color, 1);
}

void TextMesh::SetFontStyle(FontStyle newFontStyle)
{
	TTF_SetFontStyle(font.get(), newFontStyle);

	textTexture = UIManager::LoadText(font, text, color, 1);
}

void TextMesh::SetText(std::string newText, unsigned int newLinesNumber)
{
	if (text == newText && linesNumber == newLinesNumber)
		return;

	text = newText;
	linesNumber = newLinesNumber;

	textTexture = UIManager::LoadText(font, text, color, linesNumber);

	int width = 0, height = 0;
	SDL_QueryTexture(textTexture.get(), NULL, NULL, &width, &height);

	textRectSize.x = static_cast<int>(width);
	textRectSize.y = static_cast<int>(height);
}

std::string TextMesh::GetText()
{
	return text;
}

SDL_Rect TextMesh::GetTextBoundingBox()
{
	auto currentPosition = transform->GetWorldPosition();
	auto scaledSize = textRectSize * transform->GetWorldScale();

	return SDL_Rect
	{
		static_cast<int>(currentPosition.x - ((scaledSize.x) / 2.0f)),
		static_cast<int>(currentPosition.y - ((scaledSize.y) / 2.0f)),
		static_cast<int>(scaledSize.x),
		static_cast<int>(scaledSize.y)
	};
}