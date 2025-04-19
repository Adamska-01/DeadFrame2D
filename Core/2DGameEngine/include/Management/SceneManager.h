#pragma once
#include "Management/Scene.h"
#include <memory>


class SceneManager
{
private:
	static std::unique_ptr<Scene> currentGameScene;


public:
	SceneManager();

	~SceneManager();


	void UpdateScene(float deltaTime) const;

	void DrawScene() const;


	static void LoadScene(std::unique_ptr<Scene> newGameScene);

	template <typename T>
	static T* FindObjectOfType();
};


template<typename T>
inline T* SceneManager::FindObjectOfType()
{
	return currentGameScene->FindObjectOfType<T>();
}