#pragma once
#include "Core/Services/Events/Abstractions/IEventProcessor.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>


namespace DeadFrame2D::Data
{
	struct ButtonComponentModel;
}


namespace DeadFrame2D::Engine
{
	class DF2D_API Button : public IInteractableUI, public DeadFrame2D::Core::IEventProcessor
	{
		TYPE_INFO(Button, IInteractableUI);


	private:
		DeadFrame2D::Utilities::MultiCastVoid onPressedCallback;

		DeadFrame2D::Utilities::MultiCastVoid onEnterCallback;

		std::shared_ptr<SDL_Texture> currentButtonImage;

		std::shared_ptr<SDL_Texture> buttonIdleImage;

		std::shared_ptr<SDL_Texture> buttonHoveredImage;

		std::shared_ptr<SDL_Texture> buttonPressedImage;

		SDL_Color idleFillColor;

		SDL_Color hoveredFillColor;

		SDL_Color pressedFillColor;


		std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	public:
		Button(const DeadFrame2D::Data::ButtonComponentModel& buttonConfiguration);

		virtual ~Button() override = default;


		void Init() override;

		void Draw() override;


		virtual void OnPointerEnter() override;

		virtual void OnPointerExit() override;

		virtual void OnPointerDown() override;

		virtual void OnPointerUp() override;


		void AddPressedCallback(const ComponentHandleBase& handle, std::function<void()> onPressedHandler);

		void AddEnterCallback(const ComponentHandleBase& handle, std::function<void()> onEnterHandler);

		SDL_FRect GetBoundingBox() const;

		SDL_Color GetIdleFillColor() const;

		SDL_Color GetHoveredFillColor() const;

		SDL_Color GetPressedFillColor() const;

		void SetIdleFillColor(const SDL_Color& color);

		void SetHoveredFillColor(const SDL_Color& color);

		void SetPressedFillColor(const SDL_Color& color);

		void SetIdleButtonImageSource(std::string_view idleButtonSource);

		void SetHoveredButtonImageSource(std::string_view hoveredButtonSource);

		void SetPressedButtonImageSource(std::string_view pressedButtonSource);
	};
}