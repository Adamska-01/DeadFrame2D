#include "Components/Transform.h"
#include "Components/UI/Button.h"
#include "GameObject.h"
#include "SubSystems/Events/EventManager.h"
#include "Tools/Collisions/CollisionHandler.h"


Button::Button(
	std::function<void()> onPressedHandler, 
	std::string_view idleButtonSource, 
	std::string_view hoveredButtonSource, 
	std::string_view pressedButtonSource, 
	Vector2F size)
{
	isPressed = false;
	isHovered = false;
	transfom = nullptr;

	destRect.w = static_cast<int>(size.x);
	destRect.h = static_cast<int>(size.y);

	AddPressedCallback(onPressedHandler);
	SetButtonImageSources(idleButtonSource, hoveredButtonSource, pressedButtonSource);

	EventManager::AddEventProcessor(this);
}

Button::~Button()
{
	EventManager::RemoveEventProcessor(this);
}

std::optional<int> Button::ProcessEvents(const SDL_Event& sdlEvent)
{
	switch (sdlEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT)
		{
			Vector2F mousePos{
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
			Vector2F mousePos{
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
	destRect.x = static_cast<int>(transfom->position.x - (destRect.w / 2.0f));
	destRect.y = static_cast<int>(transfom->position.y - (destRect.h / 2.0f));
}

void Button::Draw()
{
	TextureManager::DrawTexture(currentButtonImage, NULL, &destRect);
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
		currentButtonImage = buttonHoveredImage;
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