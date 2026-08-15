#include "Core/Context/Systems/Coroutines/CoroutineScheduler.h"
#include "Data/Services/ServiceContext.h"
#include "Data/Systems/CoreContext.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/ECS/System/Scene/SceneManager.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Engine/ECS/System/Scene/TestScene.h"
#include "Mocks/Services/Time/MockTimeProvider.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	// SceneManager's lifecycle methods are protected (only DeadFrameRuntime is a friend);
	// this subclass republishes them as public so the tests below can drive them directly.
	struct TestSceneManager : SceneManager
	{
		using SceneManager::SetContexts;
		using SceneManager::UpdateScene;
		using SceneManager::LateUpdateScene;
		using SceneManager::DrawScene;
		using SceneManager::LoadNewSceneIfAvailable;
	};


	// Bundles the handful of dependencies SceneManager itself touches
	// (coroutineScheduler->Reset(), frameTimer->SetTimeScale) so each test can build
	// one without repeating the wiring. EventDispatcher is TestScene's, not SceneManager's.
	struct Fixture
	{
		MockTimeProvider timeProvider;
		CoroutineScheduler coroutineScheduler{ &timeProvider };
		EventDispatcher dispatcher;
		TestSceneManager manager;

		Fixture()
		{
			manager.SetContexts(
				CoreContext{ .coroutineScheduler = &coroutineScheduler },
				ServiceContext{ .frameTimer = &timeProvider });
		}
	};
}


TEST_SUITE_BEGIN("SceneManager");


TEST_CASE("LoadNewSceneIfAvailable returns false when nothing was loaded")
{
	Fixture fixture;

	CHECK_FALSE(fixture.manager.LoadNewSceneIfAvailable());
	CHECK(SceneManager::GetActiveScene() == nullptr);
}

TEST_CASE("LoadScene + LoadNewSceneIfAvailable constructs and enters the new scene")
{
	Fixture fixture;

	SceneManager::LoadScene<TestScene>(&fixture.dispatcher);

	CHECK(fixture.manager.LoadNewSceneIfAvailable());
	CHECK(SceneManager::GetActiveScene() != nullptr);
}

TEST_CASE("LoadNewSceneIfAvailable resets the time scale to 1")
{
	Fixture fixture;
	fixture.timeProvider.timeScale = 0.0f;

	SceneManager::LoadScene<TestScene>(&fixture.dispatcher);
	fixture.manager.LoadNewSceneIfAvailable();

	CHECK(fixture.timeProvider.timeScale == doctest::Approx(1.0f));
}

TEST_CASE("Consuming the pending factory clears it - a second call is a no-op until LoadScene runs again")
{
	Fixture fixture;

	SceneManager::LoadScene<TestScene>(&fixture.dispatcher);

	CHECK(fixture.manager.LoadNewSceneIfAvailable());
	CHECK_FALSE(fixture.manager.LoadNewSceneIfAvailable());
}

TEST_CASE("Loading a second scene tears down the first, invalidating its objects")
{
	Fixture fixture;

	SceneManager::LoadScene<TestScene>(&fixture.dispatcher);
	fixture.manager.LoadNewSceneIfAvailable();

	auto objInFirstScene = GameObject::Instantiate<TestGameObject>();
	REQUIRE(objInFirstScene != nullptr);

	SceneManager::LoadScene<TestScene>(&fixture.dispatcher);
	fixture.manager.LoadNewSceneIfAvailable();

	CHECK(SceneManager::GetActiveScene() != nullptr);
	CHECK(objInFirstScene == nullptr);
}

TEST_CASE("GameObject::Instantiate throws when no scene is active")
{
	Fixture fixture;

	CHECK_THROWS_AS(GameObject::Instantiate<TestGameObject>(), std::runtime_error);
}

TEST_CASE("GameObject::Instantiate spawns into the currently active scene")
{
	Fixture fixture;

	SceneManager::LoadScene<TestScene>(&fixture.dispatcher);
	fixture.manager.LoadNewSceneIfAvailable();

	auto obj = GameObject::Instantiate<TestGameObject>();

	REQUIRE(obj != nullptr);
	CHECK(obj->GetTransform() != nullptr);
}

TEST_CASE("UpdateScene/LateUpdateScene/DrawScene are no-ops with no active scene")
{
	Fixture fixture;

	CHECK_NOTHROW(fixture.manager.UpdateScene(0.016f));
	CHECK_NOTHROW(fixture.manager.LateUpdateScene(0.016f));
	CHECK_NOTHROW(fixture.manager.DrawScene());
}

TEST_CASE("SetContexts is reflected by GetCoreContext/GetServiceContext")
{
	Fixture fixture;

	CHECK(SceneManager::GetCoreContext().coroutineScheduler == &fixture.coroutineScheduler);
	CHECK(SceneManager::GetServiceContext().frameTimer == &fixture.timeProvider);
}

TEST_CASE("Constructing a second SceneManager while one is active throws")
{
	Fixture fixture;

	CHECK_THROWS_AS(SceneManager{}, std::runtime_error);

	// The failed construction must not have disturbed the still-active instance.
	CHECK(SceneManager::GetCoreContext().coroutineScheduler == &fixture.coroutineScheduler);
}

TEST_CASE("A new SceneManager can be constructed after the previous one is destroyed")
{
	{
		Fixture fixture;
	}

	CHECK_NOTHROW(SceneManager{});
}


TEST_SUITE_END();