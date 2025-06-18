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

	std::shared_ptr<SDL_Texture> currentButtonImage;

	std::shared_ptr<SDL_Texture> buttonIdleImage;

	std::shared_ptr<SDL_Texture> buttonHoveredImage;

	std::shared_ptr<SDL_Texture> buttonPressedImage;


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
	
	SDL_Rect GetBoundingBox() const;

	void SetButtonImageSources(std::string_view idleButtonSource, std::string_view hoveredButtonSource, std::string_view pressedButtonSource);
};