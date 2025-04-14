#pragma once
#include "GameScene.h"
#include <memory>


class GameObjectRegistry
{
private:
	static std::unique_ptr<GameScene> currentGameScene;


public:
	GameObjectRegistry();

	~GameObjectRegistry();


	void Update(float deltaTime) const;

	void Draw() const;


	template <typename T>
	static T* FindObjectOfType();
	
	static void LoadScene(std::unique_ptr<GameScene> newGameScene);
};


template<typename T>
inline T* GameObjectRegistry::FindObjectOfType()
{
	return currentGameScene->FindObjectOfType<T>();
}