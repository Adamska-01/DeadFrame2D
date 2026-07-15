#include "Constants/CommonColors.h"
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Input/Devices/DeviceManager.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/MouseInputDevice.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Data/Blueprints/UI/Button/ButtonComponentModel.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Button.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Models;
	using namespace DF2D::Utilities;


	Button::Button(const ButtonComponentModel& buttonConfiguration)
	{
		isPressed = false;
		isHovered = false;

		this->widgetSize = buttonConfiguration.buttonSize;

		if (buttonConfiguration.onPressedHandler)
		{
			const auto& handler = *buttonConfiguration.onPressedHandler;
			AddPressedCallback(handler.handle, handler.callback);
		}

		if (buttonConfiguration.onEnterHandler)
		{
			const auto& handler = *buttonConfiguration.onEnterHandler;
			AddEnterCallback(handler.handle, handler.callback);
		}

		idleSource = buttonConfiguration.idleButtonSource;
		hoveredSource = buttonConfiguration.hoveredButtonSource;
		pressedSource = buttonConfiguration.pressedButtonSource;

		idleFillColor = CommonColors::GRAY;
		hoveredFillColor = CommonColors::LIGHT_GRAY;
		pressedFillColor = CommonColors::DARK_GRAY;
	}

	void Button::ProcessPointer()
	{
		auto* mouse = input->Devices()->Mouse();

		if (mouse == nullptr)
			return;

		auto mousePos = mouse->GetMousePosition();
		auto destRect = GetBoundingBox();

		auto isColliding = Collision::PointVsBox(mousePos, destRect);

		auto mouseDelta = mouse->GetMouseDelta();
		auto mouseMoved = mouseDelta.x != 0.0f || mouseDelta.y != 0.0f;

		if (mouseMoved)
		{
			if (isColliding && !isHovered && !isPressed)
			{
				OnPointerEnter();
			}
			else if (!isColliding && isHovered && !isPressed)
			{
				OnPointerExit();
			}
		}

		auto leftButton = mouse->GetButtonState(MouseButtonCode::LEFT);

		if (leftButton.pressed && isColliding)
		{
			OnPointerDown();
		}

		if (leftButton.released)
		{
			if (isColliding)
			{
				OnPointerUp();
			}
			else
			{
				isPressed = false;
				currentButtonImage = buttonIdleImage;
			}
		}
	}

	void Button::Init()
	{
		UIComponent::Init();

		textureManager = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().textureManager, NAME_OF(textureManager));
		input = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().input, NAME_OF(input));

		SetIdleButtonImageSource(idleSource);
		SetHoveredButtonImageSource(hoveredSource);
		SetPressedButtonImageSource(pressedSource);
	}

	void Button::Update(float deltaTime)
	{
		UIComponent::Update(deltaTime);

		if (!IsActive() || input == nullptr)
			return;

		ProcessPointer();
	}

	void Button::Draw()
	{
		auto destRect = GetBoundingBox();

		renderTask.renderPhase = parentCanvas->GetRenderMode() == CanvasRenderMode::SCREEN_SPACE_CAMERA ? RenderPhase::SCREEN_SPACE_CAMERA_UI : RenderPhase::SCREEN_SPACE_OVERLAY_UI;
		renderTask.sortOrder = parentCanvas->GetSortOrder();
		renderTask.canvas = parentCanvas;

		if (currentButtonImage)
		{
			renderTask.renderData = SpriteRenderData
			{
				.texture = currentButtonImage,
				.destRect = destRect,
				.rotation = transform->GetWorldRotation()
			};
		}
		else
		{
			Color fillColor;

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

			renderTask.renderData = RectRenderData
			{
				.filled = true,
				.destRect = destRect,
				.rotation = transform->GetWorldRotation(),
				.color = fillColor
			};
		}

		RenderSystem::Submit(renderTask);
	}

	void Button::OnPointerEnter()
	{
		isHovered = true;

		onEnterCallback.Broadcast();

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
			onPressedCallback.Broadcast();

			currentButtonImage = buttonHoveredImage;
		}
		else
		{
			currentButtonImage = buttonPressedImage;
		}
	}

	void Button::AddPressedCallback(const ComponentHandleBase& handle, std::function<void()> onPressedHandler)
	{
		this->onPressedCallback.AddHandle(handle, onPressedHandler);
	}

	void Button::AddEnterCallback(const ComponentHandleBase& handle, std::function<void()> onEnterHandler)
	{
		this->onEnterCallback.AddHandle(handle, onEnterHandler);
	}

	RectF Button::GetBoundingBox() const
	{
		auto currentPosition = transform->GetWorldPosition();
		auto scaledSize = GetWidgetSize();
		auto anchorVector = GetAnchorFromPreset(anchor);

		return RectF
		{
			.x = currentPosition.x - (scaledSize.x * anchorVector.x),
			.y = currentPosition.y - (scaledSize.y * anchorVector.y),
			.w = scaledSize.x,
			.h = scaledSize.y
		};
	}

	Color Button::GetIdleFillColor() const
	{
		return idleFillColor;
	}

	Color Button::GetHoveredFillColor() const
	{
		return hoveredFillColor;
	}

	Color Button::GetPressedFillColor() const
	{
		return pressedFillColor;
	}

	void Button::SetIdleFillColor(const Color& color)
	{
		idleFillColor = color;
	}

	void Button::SetHoveredFillColor(const Color& color)
	{
		hoveredFillColor = color;
	}

	void Button::SetPressedFillColor(const Color& color)
	{
		pressedFillColor = color;
	}

	void Button::SetIdleButtonImageSource(std::string_view idleButtonSource)
	{
		idleSource = idleButtonSource;

		if (textureManager && !idleButtonSource.empty())
		{
			currentButtonImage = buttonIdleImage = textureManager->LoadTexture(idleButtonSource);
		}
	}

	void Button::SetHoveredButtonImageSource(std::string_view hoveredButtonSource)
	{
		hoveredSource = hoveredButtonSource;

		if (textureManager && !hoveredButtonSource.empty())
		{
			buttonHoveredImage = textureManager->LoadTexture(hoveredButtonSource);
		}
	}

	void Button::SetPressedButtonImageSource(std::string_view pressedButtonSource)
	{
		pressedSource = pressedButtonSource;

		if (textureManager && !pressedButtonSource.empty())
		{
			buttonPressedImage = textureManager->LoadTexture(pressedButtonSource);
		}
	}
}