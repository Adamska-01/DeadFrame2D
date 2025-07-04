#pragma once
#include "Components/UI/Abstractions/IInteractableUI.h"
#include "SubSystems//Events/Interfaces/IEventProcessor.h"
#include "Tools/MulticastDelegate.h"
#include <functional>


class Transform;
struct ButtonComponentModel;


class Button : public IInteractableUI, public IEventProcessor
{
private:
	MultiCastVoid onPressedCallback;

	MultiCastVoid onEnterCallback;

	std::shared_ptr<SDL_Texture> currentButtonImage;

	std::shared_ptr<SDL_Texture> buttonIdleImage;

	std::shared_ptr<SDL_Texture> buttonHoveredImage;

	std::shared_ptr<SDL_Texture> buttonPressedImage;

	SDL_Color idleFillColor;

	SDL_Color hoveredFillColor;

	SDL_Color pressedFillColor;


public:
	Button(const ButtonComponentModel& buttonConfiguration);

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