#pragma once
#include "Core/CoreEvents/Abstractions/IEventProcessor.h"
#include "DF2D_API.h"
#include "Engine/Components/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>


namespace DeadFrame2D::Data
{
	struct ButtonComponentModel;
}


namespace DeadFrame2D::Engine
{
	class Transform;


	class DF2D_API Button : public IInteractableUI, public DeadFrame2D::Core::IEventProcessor
	{
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


	public:
		Button(const DeadFrame2D::Data::ButtonComponentModel& buttonConfiguration);

		virtual ~Button() override = default;


		virtual std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


		virtual void Init() override;

		virtual void Start() override;

		virtual void Update(float deltaTime) override;

		virtual void Draw() override;


		virtual void OnPointerEnter() override;

		virtual void OnPointerExit() override;

		virtual void OnPointerDown() override;

		virtual void OnPointerUp() override;


		void AddPressedCallback(std::function<void()> onPressedHandler, std::uintptr_t identifier);

		void AddEnterCallback(std::function<void()> onEnterCallback, std::uintptr_t identifier);

		SDL_Rect GetBoundingBox() const;

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