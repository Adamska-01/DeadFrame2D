#pragma once
#include "Management/Scene.h"
#include <memory>


class SceneManager
{
	friend class Engine;


private:
	static std::unique_ptr<Scene> currentGameScene;


	void UpdateScene(float deltaTime) const;

	void DrawScene() const;


public:
	SceneManager();

	~SceneManager();


	static void LoadScene(std::unique_ptr<Scene> newGameScene);

	template <typename T>
	static T* FindObjectOfType();
};


template<typename T>
inline T* SceneManager::FindObjectOfType()
{
	return currentGameScene->FindObjectOfType<T>();
}