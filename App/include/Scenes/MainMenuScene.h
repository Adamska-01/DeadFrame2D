#pragma once
#include <Management/Scene.h>
#include <Math/Vector2.h>
#include <memory>


class ButtonBlueprint;


class MainMenuScene : public Scene
{
private:
	void CreateBackground(const Vector2I& renderTargetSize);
	
	void CreateTitle(const Vector2I& renderTargetSize);
	
	void SetupLayoutGroup(const Vector2I& renderTargetSize);
	
	std::weak_ptr<ButtonBlueprint> CreateButton(const std::string& text, const std::function<void()>& onPressedHandler);


public:
	MainMenuScene() = default;

	virtual ~MainMenuScene() override = default;


	virtual void Enter() override;
};