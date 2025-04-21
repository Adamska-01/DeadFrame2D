#include "Components/Transform.h"
#include "Components/UI/Button.h"
#include "GameObject.h"
#include "SubSystems/Events/EventManager.h"
#include "SubSystems/UIManager.h"
#include "Tools/Collisions/CollisionHandler.h"


Button::Button(
	std::function<void()> onPressedHandler, 
	std::string_view idleButtonSource, 
	std::string_view hoveredButtonSource, 
	std::string_view pressedButtonSource, 
	std::string buttonText,
	Vector2F size)
{
	isPressed = false;
	isHovered = false;
	transfom = nullptr;

	destRect.w = static_cast<int>(size.x);
	destRect.h = static_cast<int>(size.y);

	text = buttonText;

	AddPressedCallback(onPressedHandler);
	SetButtonImageSources(idleButtonSource, hoveredButtonSource, pressedButtonSource);
}

std::optional<int> Button::ProcessEvents(const SDL_Event& sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT)
		{
			auto mousePos = Vector2F{
				static_cast<float>(sdlEvent.button.x),
				static_cast<float>(sdlEvent.button.y)
			};

			if (CollisionHandler::PointVsBox(mousePos, &destRect))
			{
				OnPointerDown();
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT)
		{
			auto mousePos = Vector2F{
				static_cast<float>(sdlEvent.button.x),
				static_cast<float>(sdlEvent.button.y)
			};

			if (CollisionHandler::PointVsBox(mousePos, &destRect))
			{
				OnPointerUp();
			}
			else
			{
				isPressed = false;
				currentButtonImage = buttonIdleImage;
			}
		}
		break;
	
	case SDL_EventType::SDL_MOUSEMOTION:
		{
			auto isColliding = CollisionHandler::PointVsBox(
				Vector2F(
					static_cast<float>(sdlEvent.motion.x),
					static_cast<float>(sdlEvent.motion.y)),
				&destRect);

			if (isColliding && !isHovered)
			{
				OnPointerEnter();
			}
			else if (!isColliding && isHovered)
			{
				OnPointerExit();
			}
		}
		break;
	}

	return std::nullopt;
}

void Button::Init()
{
	transfom = OwningObject->GetComponent<Transform>();
}

void Button::Update(float deltaTime)
{
	auto currentPosition = transfom->GetWorldPosition();

	destRect.x = static_cast<int>(currentPosition.x - (destRect.w / 2.0f));
	destRect.y = static_cast<int>(currentPosition.y - (destRect.h / 2.0f));
}

void Button::Draw()
{
	TextureManager::DrawTexture(currentButtonImage, NULL, &destRect);

	auto currentPosition = transfom->GetWorldPosition();

	// This is not final. just testing text
	auto font = UIManager::LoadFont("App/Assets/Fonts/consola.ttf", 100);
	auto texture = UIManager::LoadText(font.get(), text, SDL_Color(216, 139, 13, 255), 1);
	TTF_SetFontStyle(font.get(), TTF_STYLE_BOLD);
	auto textDest = destRect;

	textDest.w = static_cast<int>(textDest.w * 0.5f);
	textDest.h = static_cast<int>(textDest.h * 0.5f);
	textDest.x = static_cast<int>(currentPosition.x - (textDest.w / 2.0f));
	textDest.y = static_cast<int>(currentPosition.y - (textDest.h / 2.0f));
	TextureManager::DrawTexture(std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture), NULL, &textDest);
}

void Button::AddPressedCallback(std::function<void()> onPressedHandler)
{
	this->onPressedCallback += onPressedHandler;
}

const SDL_Rect& Button::GetBoundingBox() const
{
	return destRect;
}

void Button::SetButtonImageSources(std::string_view idleButtonSource, std::string_view hoveredButtonSource, std::string_view pressedButtonSource)
{
	currentButtonImage = buttonIdleImage = TextureManager::LoadTexture(idleButtonSource);
	buttonHoveredImage = TextureManager::LoadTexture(hoveredButtonSource);
	buttonPressedImage = TextureManager::LoadTexture(pressedButtonSource);
}

void Button::SetButtonSize(int width, int height)
{
	destRect.w = width;
	destRect.h = height;
}

void Button::OnPointerEnter()
{
	isHovered = true;

	currentButtonImage = buttonHoveredImage;
}

void Button::OnPointerExit()
{
	isHovered = false;

	if (!isPressed)
	{
		currentButtonImage = buttonIdleImage;
	}
}

void Button::OnPointerDown()
{
	isPressed = true;

	currentButtonImage = buttonPressedImage;
}

void Button::OnPointerUp()
{
	isPressed = false;

	if (isHovered)
	{
		onPressedCallback();

		currentButtonImage = buttonHoveredImage;
	}
	else
	{
		currentButtonImage = buttonPressedImage;
	}
}