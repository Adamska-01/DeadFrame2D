#include "Components/Transform.h"
#include "Components/UI/TextMesh.h"
#include "GameObject.h"
#include "SubSystems/TextureManager.h"
#include "SubSystems/UIManager.h"


TextMesh::TextMesh(std::string_view fontSource, std::string text, unsigned int fontSize, unsigned int linesNumber)
	: text(text)
{
	font = UIManager::LoadFont(fontSource, fontSize);
	
	SetText(text, linesNumber);
}

void TextMesh::Init()
{
	transform = OwningObject->GetComponent<Transform>();
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

void TextMesh::SetTextColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;

	textTexture = UIManager::LoadText(font, text, color, 1);
}

void TextMesh::SetFontStyle(FontStyle newFontStyle)
{
	TTF_SetFontStyle(font.get(), newFontStyle);
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