#pragma once
#include "DF2D_API.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Engine/SceneSystem/Scene.h"
#include <memory>
#include <vector>


namespace DeadFrame2D::Core 
{ 
	class DeadFrameRuntime; 
}


namespace DeadFrame2D::Engine
{
	class DF2D_API SceneManager
	{
		friend DeadFrame2D::Core::DeadFrameRuntime;


	private:
		static std::unique_ptr<Scene> currentScene;

		static std::function<std::unique_ptr<Scene>()> newSceneFactory;


		void UpdateScene(float deltaTime) const;

		void LateUpdateScene(float deltaTime) const;

		void DrawScene() const;

		bool LoadNewSceneIfAvailable();


	public:
		SceneManager();

		~SceneManager();


		template<typename TScene, typename... Args>
		static void LoadScene(Args&&... args);

		template <typename T>
		static ComponentHandle<T> FindObjectOfType();

		template <typename T>
		static std::vector<ComponentHandle<T>> FindObjectsOfType();
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
	inline ComponentHandle<T> SceneManager::FindObjectOfType()
	{
		return currentScene->FindObjectOfType<T>();
	}

	template<typename T>
	inline std::vector<ComponentHandle<T>> SceneManager::FindObjectsOfType()
	{
		return currentScene->FindObjectsOfType<T>();
	}
}