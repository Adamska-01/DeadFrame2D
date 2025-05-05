#pragma once
#include "Components/Abstractions/MenuBase.h"
#include <Components/GameComponent.h>
#include <stack>


class MenuManager : public GameComponent
{
private:
	std::stack<MenuBase*> menuStack;


public:
	MenuManager();

	~MenuManager() = default;
	

	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void PushMenu(MenuBase* menu);

	void PopMenu();

	MenuBase* GetCurrentMenu() const;
};