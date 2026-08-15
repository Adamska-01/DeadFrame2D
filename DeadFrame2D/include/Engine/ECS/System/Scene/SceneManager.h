#pragma once
#include "Data/Services/ServiceContext.h"
#include "Data/Systems/CoreContext.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <vector>


namespace DF2D::Engine
{
	class Scene;


	class DF2D_API SceneManager
	{
		friend class GameObject;


	private:
		static SceneManager* activeInstance;


		std::shared_ptr<Scene> currentScene;

		std::function<std::shared_ptr<Scene>()> newSceneFactory;

		Data::CoreContext coreCtx;

		Data::ServiceContext serviceCtx;


		template<typename T, typename... Args>
		static ObjectHandle<T> Instantiate(Args&&... args);

		static SceneManager& Active();


	public:
		SceneManager();

		~SceneManager();


		void SetContexts(Data::CoreContext coreCtx, Data::ServiceContext serviceCtx);

		void UpdateScene(float deltaTime) const;

		void LateUpdateScene(float deltaTime) const;

		void DrawScene() const;

		bool LoadNewSceneIfAvailable();


		template<typename TScene, typename... Args>
		static void LoadScene(Args&&... args);

		template <typename T>
		static ComponentHandle<T> FindObjectOfType();

		template <typename T>
		static std::vector<ComponentHandle<T>> FindObjectsOfType();


		static const Scene* GetActiveScene();

		static Data::CoreContext GetCoreContext();

		static Data::ServiceContext GetServiceContext();
	};
}


#include "Engine/ECS/System/Scene/Scene.h"


namespace DF2D::Engine
{
	template<typename T, typename ...Args>
	inline ObjectHandle<T> SceneManager::Instantiate(Args && ...args)
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

		auto& instance = Active();

		if (!instance.currentScene)
		{
			throw std::runtime_error("There is no active scene! Load a scene before instantiating a GameObject!");
		}

		return instance.currentScene->template Instantiate<T>(instance.coreCtx, instance.serviceCtx, std::forward<Args>(args)...);
	}

	template<typename TScene, typename ...Args>
	inline void SceneManager::LoadScene(Args && ...args)
	{
		static_assert(std::is_base_of_v<Scene, TScene>, "TScene must derive from Scene");

		Active().newSceneFactory = [argsTuple = std::make_tuple(std::forward<Args>(args)...)]() mutable
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
		auto& instance = Active();

		return instance.currentScene
			? instance.currentScene->template FindObjectOfType<T>()
			: ComponentHandle<T>();
	}

	template<typename T>
	inline std::vector<ComponentHandle<T>> SceneManager::FindObjectsOfType()
	{
		auto& instance = Active();

		return instance.currentScene
			? instance.currentScene->template FindObjectsOfType<T>()
			: std::vector<ComponentHandle<T>>();
	}
}