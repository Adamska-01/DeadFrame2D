#include <SubSystems/Renderer.h>
#include <SubSystems/TextureManager.h>
#include <SubSystems/UIManager.h>
#include <UI/UIText.h>


UIText::UIText(std::string fontPath, std::string text)
	: Text(text)
{
	// TODO: Remove magic number (the parameter doesn't seem to make a difference, just make it constant)
	font = UIManager::LoadFont(fontPath, 20);
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

		UIManager::DrawText(texture, dest, scale);

#if _DEBUG
		// TODO: Remove magic numbers
		UIManager::DrawDebugTextBox(50, 80, 255, 255, dest);
#endif
	}
	else
	{
#if _DEBUG
		// TODO: Remove magic numbers
		UIManager::DrawDebugTextBox(50, 80, 255, 255, textRect);
#endif
		UIManager::DrawText(texture, textRect, scale);
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

	texture = UIManager::LoadText(font.get(), Text, color, numRows);

	SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);

	return texture;
}

SDL_Rect UIText::GetTextureDim()
{
	return textRect;
}