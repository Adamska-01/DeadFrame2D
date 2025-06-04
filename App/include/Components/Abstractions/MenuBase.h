#pragma once
#include <Components/GameComponent.h>
#include <vector>


class IInteractableUI;
class MenuManager;


class MenuBase : public GameComponent
{
private:
	void Navigate(IInteractableUI* (*getNext)(IInteractableUI*));


protected:
	MenuManager* menuManager;

	std::vector<IInteractableUI*> allInteractables;

	IInteractableUI* selectedInteractable;


public:
	MenuBase();

	virtual ~MenuBase() override = default;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	virtual void Show();

	virtual void Hide();


	//UI Navigation
	virtual void NavigateLeft();
	
	virtual void NavigateRight();
	
	virtual void NavigateUp();
	
	virtual void NavigateDown();
	
	virtual void Confirm();
	
	virtual void GoBack();
};