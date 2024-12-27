#include <SubSystems/Renderer.h>
#include <SubSystems/TextureManager.h>
#include <SubSystems/UIManager.h>
#include <UI/UIText.h>


UIText::UIText(Fonts font, std::string text)
	: font(font), Text(text)
{
	texture = UpdateText("", 1);
}

void UIText::Draw(Vector2 scale, bool center)
{
	if (center)
	{
		SDL_Rect dest
		{ 
			textRect.x - (textRect.w / 2), 
			textRect.y - (textRect.h / 2), 
			textRect.w, 
			textRect.h 
		};

		UIManager::GetInstance()->DrawText(texture, dest, scale);

#if _DEBUG
		// TODO: Remove magic numbers
		DrawTextBox(50, 80, 255, 255, dest);
#endif
	}
	else
	{
#if _DEBUG
		// TODO: Remove magic numbers
		DrawTextBox(50, 80, 255, 255, textRect);
#endif
		UIManager::GetInstance()->DrawText(texture, textRect, scale);
	}
}

void UIText::ChangeColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void UIText::SetPos(int x, int y)
{
	textRect.x = x;
	textRect.y = y;
}

void UIText::SetWidthAndHeight(int w, int h)
{
	textRect.w = w;
	textRect.h = h;
}

SDL_Texture* UIText::UpdateText(std::string newText, unsigned int numRows)
{
	if (Text == newText)
		return texture;

	Text = newText;

	texture = UIManager::GetInstance()->LoadText(font, Text, color, numRows);

	SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);

	return texture;
}

SDL_Rect UIText::GetTextureDim()
{
	return textRect;
}

void UIText::DrawTextBox(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect textRect)
{
	SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), r, g, b, a);
	SDL_RenderDrawRect(Renderer::GetInstance()->GetRenderer(), &textRect);
}