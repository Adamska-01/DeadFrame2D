#pragma once
#include "Components/GameComponent.h"
#include "Math/Vector2.h"
#include "SubSystems//Events/Interfaces/IEventProcessor.h"
#include "SubSystems/TextureManager.h"
#include "Tools/MulticastDelegate.h"
#include <functional>
#include <string_view>


class Transform;


class Button : public GameComponent, public IEventProcessor
{
private:
	bool isPressed;

	bool isHovered;

	SDL_Rect destRect;

	std::string text;

	Transform* transfom;

	MultiCastVoid onPressedCallback;

	std::shared_ptr<SDL_Texture> currentButtonImage;

	std::shared_ptr<SDL_Texture> buttonIdleImage;

	std::shared_ptr<SDL_Texture> buttonHoveredImage;

	std::shared_ptr<SDL_Texture> buttonPressedImage;


public:
	Button(
		std::function<void()> onPressedHandler,
		std::string_view idleButtonSource,
		std::string_view hoveredButtonSource,
		std::string_view pressedButtonSource,
		std::string buttonText,
		Vector2F size);

	virtual ~Button() override;


	virtual std::optional<int> ProcessEvents(const SDL_Event& sdlEvent) override;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void AddPressedCallback(std::function<void()> onPressedHandler);
	
	const SDL_Rect& GetBoundingBox() const;

	void SetButtonImageSources(std::string_view idleButtonSource, std::string_view hoveredButtonSource, std::string_view pressedButtonSource);

	void SetButtonSize(int width, int height);


	void OnPointerEnter();

	void OnPointerExit();

	void OnPointerDown();
	
	void OnPointerUp();
};