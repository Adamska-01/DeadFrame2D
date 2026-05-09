#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <vector>


namespace DF2D::Core 
{ 
	class DeadFrameRuntime; 
}


namespace DF2D::Engine
{
	class Scene;


	class DF2D_API SceneManager
	{
		friend class GameObject;
		friend Core::DeadFrameRuntime;


	private:
		static std::shared_ptr<Scene> currentScene;

		static std::function<std::shared_ptr<Scene>()> newSceneFactory;


		void UpdateScene(float deltaTime) const;

		void LateUpdateScene(float deltaTime) const;

		void DrawScene() const;

		bool LoadNewSceneIfAvailable();


		template<typename T, typename... Args>
		static ObjectHandle<T> Instantiate(Args&&... args);


	public:
		SceneManager() = default;

		~SceneManager();


		template<typename TScene, typename... Args>
		static void LoadScene(Args&&... args);

		template <typename T>
		static ComponentHandle<T> FindObjectOfType();

		template <typename T>
		static std::vector<ComponentHandle<T>> FindObjectsOfType();


		static const Scene* GetActiveScene();
	};
}


#include "Engine/ECS/System/Scene/Scene.h"


namespace DF2D::Engine
{
	template<typename T, typename ...Args>
	inline ObjectHandle<T> SceneManager::Instantiate(Args && ...args)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

		if (!currentScene)
		{
			throw std::runtime_error("There is no active scene! Load a scene before instantiating a GameObject!");
		}

		return currentScene->template Instantiate<T>(std::forward<Args>(args)...);
	}

	template<typename TScene, typename ...Args>
	inline void SceneManager::LoadScene(Args && ...args)
	{
		static_assert(std::is_base_of_v<Scene, TScene>, "TScene must derive from Scene");

		newSceneFactory = [argsTuple = std::make_tuple(std::forward<Args>(args)...)]() mutable
			{
				return std::apply([](auto&&... unpackedArgs)
					{
						return std::make_shared<TScene>(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
					},
					std::move(argsTuple));
			};
	}

	template<typename T>
	inline ComponentHandle<T> SceneManager::FindObjectOfType()
	{
		return currentScene
			? currentScene->template FindObjectOfType<T>()
			: ComponentHandle<T>();
	}

	template<typename T>
	inline std::vector<ComponentHandle<T>> SceneManager::FindObjectsOfType()
	{
		return currentScene
			? currentScene->template FindObjectsOfType<T>()
			: std::vector<ComponentHandle<T>>();
	}
}