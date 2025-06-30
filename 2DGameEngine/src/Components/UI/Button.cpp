#include "Components/Transform.h"
#include "Components/UI/Button.h"
#include "Models/Components/UI/ButtonComponentModel.h"
#include "SubSystems/TextureManager.h"
#include "Tools/Collisions/CollisionUtils.h"


Button::Button(const ButtonComponentModel& buttonConfiguration)
{
	isPressed = false;
	isHovered = false;

	this->widgetSize = buttonConfiguration.buttonSize;

	AddPressedCallback(buttonConfiguration.onPressedHandler, buttonConfiguration.identifier);
	AddEnterCallback(buttonConfiguration.onEnterHandler, buttonConfiguration.identifier);
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

			if (CollisionUtils::PointVsBox(mousePos, &destRect))
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

			if (CollisionUtils::PointVsBox(mousePos, &destRect))
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
			auto isColliding = CollisionUtils::PointVsBox(
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
	UIComponent::Init();
}

void Button::Start()
{

}

void Button::Update(float deltaTime)
{
}

void Button::Draw()
{
	auto destRect = GetBoundingBox();

	TextureManager::DrawTexture(currentButtonImage, NULL, &destRect, transform->GetWorldRotation());
}

void Button::OnPointerEnter()
{
	isHovered = true;

	onEnterCallback();

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

void Button::AddPressedCallback(std::function<void()> onPressedHandler, std::uintptr_t identifier)
{
	this->onPressedCallback.RegisterCallback(onPressedHandler, identifier);
}

void Button::AddEnterCallback(std::function<void()> onEnterCallback, std::uintptr_t identifier)
{
	this->onEnterCallback.RegisterCallback(onEnterCallback, identifier);
}

SDL_Rect Button::GetBoundingBox() const
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

void Button::SetButtonImageSources(std::string_view idleButtonSource, std::string_view hoveredButtonSource, std::string_view pressedButtonSource)
{
	currentButtonImage = buttonIdleImage = TextureManager::LoadTexture(idleButtonSource);
	buttonHoveredImage = TextureManager::LoadTexture(hoveredButtonSource);
	buttonPressedImage = TextureManager::LoadTexture(pressedButtonSource);
}