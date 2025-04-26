#include "Components/Transform.h"
#include "Components/UI/Button.h"
#include "GameObject.h"
#include "Models/Components/UI/ButtonComponentModel.h"
#include "SubSystems/TextureManager.h"
#include "Tools/Collisions/CollisionHandler.h"


Button::Button(const ButtonComponentModel& buttonConfiguration)
{
	isPressed = false;
	isHovered = false;
	transform = nullptr;

	this->buttonSize = buttonConfiguration.buttonSize;

	AddPressedCallback(buttonConfiguration.onPressedHandler);
	SetButtonImageSources(
		buttonConfiguration.idleButtonSource, 
		buttonConfiguration.hoveredButtonSource, 
		buttonConfiguration.pressedButtonSource);
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
			auto destRect = GetBoundingBox();

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
			auto destRect = GetBoundingBox();

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
			auto destRect = GetBoundingBox();
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
	transform = OwningObject->GetComponent<Transform>();
}

void Button::Update(float deltaTime)
{
}

void Button::Draw()
{
	auto destRect = GetBoundingBox();

	TextureManager::DrawTexture(currentButtonImage, NULL, &destRect, transform->GetWorldRotation());
}

void Button::AddPressedCallback(std::function<void()> onPressedHandler)
{
	this->onPressedCallback += onPressedHandler;
}

SDL_Rect Button::GetBoundingBox() const
{
	auto currentPosition = transform->GetWorldPosition();
	auto scaledSize = buttonSize * transform->GetWorldScale();

	return SDL_Rect
	{
		static_cast<int>(currentPosition.x - ((scaledSize.x) / 2.0f)),
		static_cast<int>(currentPosition.y - ((scaledSize.y) / 2.0f)),
		static_cast<int>(scaledSize.x),
		static_cast<int>(scaledSize.y)
	};
}

void Button::SetButtonImageSources(std::string_view idleButtonSource, std::string_view hoveredButtonSource, std::string_view pressedButtonSource)
{
	currentButtonImage = buttonIdleImage = TextureManager::LoadTexture(idleButtonSource);
	buttonHoveredImage = TextureManager::LoadTexture(hoveredButtonSource);
	buttonPressedImage = TextureManager::LoadTexture(pressedButtonSource);
}

void Button::SetButtonSize(Vector2F size)
{
	this->buttonSize = size;
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