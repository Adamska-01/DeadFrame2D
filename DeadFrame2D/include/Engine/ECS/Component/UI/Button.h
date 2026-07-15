#pragma once
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>
#include <string>


namespace DF2D::Core
{
	class Input;
	class TextureManager;
}

namespace DF2D::Data
{
	struct ButtonComponentModel;
}


namespace DF2D::Engine
{
	class DF2D_API Button : public IInteractableUI
	{
		TYPE_INFO(Button, IInteractableUI);


	private:
		Utilities::MultiCastVoid onPressedCallback;

		Utilities::MultiCastVoid onEnterCallback;

		Data::TextureID currentButtonImage = 0;

		Data::TextureID buttonIdleImage = 0;

		Data::TextureID buttonHoveredImage = 0;

		Data::TextureID buttonPressedImage = 0;

		Core::TextureManager* textureManager = nullptr;

		Core::Input* input = nullptr;

		std::string idleSource;

		std::string hoveredSource;

		std::string pressedSource;

		Core::Color idleFillColor;

		Core::Color hoveredFillColor;

		Core::Color pressedFillColor;


		void ProcessPointer();


	public:
		Button(const Data::ButtonComponentModel& buttonConfiguration);

		virtual ~Button() override = default;


		void Init() override;

		void Update(float deltaTime) override;

		void Draw() override;


		virtual void OnPointerEnter() override;

		virtual void OnPointerExit() override;

		virtual void OnPointerDown() override;

		virtual void OnPointerUp() override;


		void AddPressedCallback(const ComponentHandleBase& handle, std::function<void()> onPressedHandler);

		void AddEnterCallback(const ComponentHandleBase& handle, std::function<void()> onEnterHandler);

		Core::RectF GetBoundingBox() const;

		Core::Color GetIdleFillColor() const;

		Core::Color GetHoveredFillColor() const;

		Core::Color GetPressedFillColor() const;

		void SetIdleFillColor(const Core::Color& color);

		void SetHoveredFillColor(const Core::Color& color);

		void SetPressedFillColor(const Core::Color& color);

		void SetIdleButtonImageSource(std::string_view idleButtonSource);

		void SetHoveredButtonImageSource(std::string_view hoveredButtonSource);

		void SetPressedButtonImageSource(std::string_view pressedButtonSource);
	};
}