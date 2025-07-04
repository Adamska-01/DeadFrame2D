#include "Components/Transform.h"
#include "Components/UI/Button.h"
#include "Constants/CommonColors.h"
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
	
	SetIdleButtonImageSource(buttonConfiguration.idleButtonSource);
	SetHoveredButtonImageSource(buttonConfiguration.hoveredButtonSource);
	SetPressedButtonImageSource(buttonConfiguration.pressedButtonSource);

	idleFillColor = CommonColors::GRAY;
	hoveredFillColor = CommonColors::LIGHT_GRAY;
	pressedFillColor = CommonColors::DARK_GRAY;
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

	if (currentButtonImage)
	{
		TextureManager::DrawTexture(currentButtonImage, nullptr, &destRect, transform->GetWorldRotation());
	}
	else
	{
		SDL_Color fillColor;

		if (isPressed)
		{
			fillColor = pressedFillColor;
		}
		else if (isHovered)
		{
			fillColor = hoveredFillColor;
		}
		else
		{
			fillColor = idleFillColor;
		}

		TextureManager::DrawRect(destRect, transform->GetWorldRotation(), fillColor, true);
	}
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

SDL_Color Button::GetIdleFillColor() const
{
	return idleFillColor;
}

SDL_Color Button::GetHoveredFillColor() const
{
	return hoveredFillColor;
}

SDL_Color Button::GetPressedFillColor() const
{
	return pressedFillColor;
}

void Button::SetIdleFillColor(const SDL_Color& color)
{
	idleFillColor = color;
}

void Button::SetHoveredFillColor(const SDL_Color& color)
{
	hoveredFillColor = color;
}

void Button::SetPressedFillColor(const SDL_Color& color)
{
	pressedFillColor = color;
}

void Button::SetIdleButtonImageSource(std::string_view idleButtonSource)
{
	if (!idleButtonSource.empty())
	{
		currentButtonImage = buttonIdleImage = TextureManager::LoadTexture(idleButtonSource);
	}
}

void Button::SetHoveredButtonImageSource(std::string_view hoveredButtonSource)
{
	if (!hoveredButtonSource.empty())
	{
		buttonHoveredImage = TextureManager::LoadTexture(hoveredButtonSource);
	}
}

void Button::SetPressedButtonImageSource(std::string_view pressedButtonSource)
{
	if (!pressedButtonSource.empty())
	{
		buttonPressedImage = TextureManager::LoadTexture(pressedButtonSource);
	}
}