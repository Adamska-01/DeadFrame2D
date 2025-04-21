#pragma once
#include "Management/Scene.h"
#include <memory>


class SceneManager
{
	friend class Engine;


private:
	static std::unique_ptr<Scene> currentScene;

	static std::function<std::unique_ptr<Scene>()> newSceneFactory;


	void UpdateScene(float deltaTime) const;

	void DrawScene() const;

	void LoadNewSceneIfAvailable();


public:
	SceneManager();

	~SceneManager();


	template<typename TScene, typename... Args>
	static void LoadScene(Args&&... args);

	template <typename T>
	static T* FindObjectOfType();
};


template<typename TScene, typename ...Args>
inline void SceneManager::LoadScene(Args && ...args)
{
	static_assert(std::is_base_of_v<Scene, TScene>, "TScene must derive from Scene");

	newSceneFactory = [argsTuple = std::make_tuple(std::forward<Args>(args)...)]() mutable 
		{
			return std::apply([](auto&&... unpackedArgs) 
				{
					return std::make_unique<TScene>(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
				}, 
				std::move(argsTuple));
		};
}

template<typename T>
inline T* SceneManager::FindObjectOfType()
{
	return currentScene->FindObjectOfType<T>();
}