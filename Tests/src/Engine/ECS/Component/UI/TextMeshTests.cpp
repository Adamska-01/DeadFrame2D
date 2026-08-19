#include "Core/Context/Systems/UI/UIManager.h"
#include "Data/Blueprints/UI/Text/TextMeshComponentModel.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/TextMesh.h"
#include "Mocks/Context/Systems/UI/MockTextBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>
#include <stdexcept>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	struct TextMeshFixture
	{
		std::unique_ptr<UIManager> uiManager;

		MockTextBackend* backend;

		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> canvasOwner;

		ObjectHandle<TestGameObject> owner;


		TextMeshFixture()
		{
			auto mock = std::make_unique<MockTextBackend>();
			backend = mock.get();
			uiManager = std::make_unique<UIManager>(std::move(mock));

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .uiManager = uiManager.get() });

			canvasOwner = scene->Create<TestGameObject>();
			canvasOwner->AddComponent<Canvas>();

			owner = scene->Create<TestGameObject>();
			owner->SetParent(canvasOwner);
		}
	};


	// The model's real default fontSource is a ResolvedPath that hits the filesystem
	// on first use; tests never touch a real backend, so always pass a plain string.
	TextMeshComponentModel MakeModel()
	{
		return TextMeshComponentModel{ .fontSource = "font.ttf" };
	}
}


TEST_SUITE_BEGIN("TextMesh");


TEST_CASE("Init builds the text texture via UIManager")
{
	TextMeshFixture fixture;
	fixture.backend->nextTextureSize = { 320, 48 };

	auto model = MakeModel();
	model.text = "Hello";
	auto textMesh = fixture.owner->AddComponent<TextMesh>(model);
	textMesh->Init();

	CHECK(fixture.backend->createTextTextureCount == 1);
	REQUIRE(fixture.backend->lastLines.size() == 1);
	CHECK(fixture.backend->lastLines[0] == "Hello");
	CHECK(textMesh->GetText() == "Hello");
}

TEST_CASE("SetFontSize rebuilds the text texture")
{
	TextMeshFixture fixture;

	auto textMesh = fixture.owner->AddComponent<TextMesh>(MakeModel());
	textMesh->Init();

	auto countAfterInit = fixture.backend->createTextTextureCount;

	textMesh->SetFontSize(48);

	CHECK(fixture.backend->createTextTextureCount == countAfterInit + 1);
	CHECK(fixture.backend->lastFontSize == 48);
}

TEST_CASE("SetTextColor rebuilds the text texture")
{
	TextMeshFixture fixture;

	auto textMesh = fixture.owner->AddComponent<TextMesh>(MakeModel());
	textMesh->Init();

	auto countAfterInit = fixture.backend->createTextTextureCount;

	textMesh->SetTextColor(Color{ 1, 2, 3, 255 });

	CHECK(fixture.backend->createTextTextureCount == countAfterInit + 1);
	CHECK(fixture.backend->lastColor == Color{ 1, 2, 3, 255 });
}

TEST_CASE("SetText rebuilds the text texture with the new text")
{
	TextMeshFixture fixture;

	auto model = MakeModel();
	model.text = "old";
	auto textMesh = fixture.owner->AddComponent<TextMesh>(model);
	textMesh->Init();

	textMesh->SetText("new");

	CHECK(textMesh->GetText() == "new");
	REQUIRE(fixture.backend->lastLines.size() == 1);
	CHECK(fixture.backend->lastLines[0] == "new");
}

TEST_CASE("SetIsCentered rebuilds and forwards the centered flag")
{
	TextMeshFixture fixture;

	auto model = MakeModel();
	model.isCentered = false;
	auto textMesh = fixture.owner->AddComponent<TextMesh>(model);
	textMesh->Init();

	textMesh->SetIsCentered(true);

	CHECK(fixture.backend->lastCenterText == true);
}

TEST_CASE("Setter called before Init defers the backend call but keeps the new value")
{
	TextMeshFixture fixture;

	auto textMesh = fixture.owner->AddComponent<TextMesh>(MakeModel());

	CHECK_NOTHROW(textMesh->SetFontSize(48));
	CHECK(fixture.backend->createTextTextureCount == 0);

	textMesh->Init();

	CHECK(fixture.backend->createTextTextureCount == 1);
	CHECK(fixture.backend->lastFontSize == 48);
}

TEST_CASE("GetTextDestRect centers the texture on the transform position by default")
{
	TextMeshFixture fixture;
	fixture.backend->nextTextureSize = { 320, 48 };

	auto textMesh = fixture.owner->AddComponent<TextMesh>(MakeModel());
	textMesh->Init();

	auto rect = textMesh->GetTextDestRect();

	CHECK(rect.x == doctest::Approx(-160.0f));
	CHECK(rect.y == doctest::Approx(-24.0f));
	CHECK(rect.w == doctest::Approx(320.0f));
	CHECK(rect.h == doctest::Approx(48.0f));
}

TEST_CASE("GetTextDestRect honors a TOP_LEFT anchor")
{
	TextMeshFixture fixture;
	fixture.backend->nextTextureSize = { 320, 48 };

	auto textMesh = fixture.owner->AddComponent<TextMesh>(MakeModel());
	textMesh->Init();
	textMesh->SetAnchor(UIAnchor::TOP_LEFT);

	auto rect = textMesh->GetTextDestRect();

	CHECK(rect.x == doctest::Approx(0.0f));
	CHECK(rect.y == doctest::Approx(0.0f));
}

TEST_CASE("GetTextDestRect honors a BOTTOM_RIGHT anchor")
{
	TextMeshFixture fixture;
	fixture.backend->nextTextureSize = { 320, 48 };

	auto textMesh = fixture.owner->AddComponent<TextMesh>(MakeModel());
	textMesh->Init();
	textMesh->SetAnchor(UIAnchor::BOTTOM_RIGHT);

	auto rect = textMesh->GetTextDestRect();

	CHECK(rect.x == doctest::Approx(-320.0f));
	CHECK(rect.y == doctest::Approx(-48.0f));
}

TEST_CASE("Draw does not throw once parented under a Canvas")
{
	TextMeshFixture fixture;

	auto textMesh = fixture.owner->AddComponent<TextMesh>(MakeModel());
	textMesh->Init();
	textMesh->Update(0.016f);

	CHECK_NOTHROW(textMesh->Draw());
}

TEST_CASE("Update throws when there is no Canvas ancestor")
{
	TextMeshFixture fixture;

	auto orphan = fixture.scene->Create<TestGameObject>();
	auto textMesh = orphan->AddComponent<TextMesh>(MakeModel());
	textMesh->Init();

	CHECK_THROWS_AS(textMesh->Update(0.016f), std::invalid_argument);
}


TEST_SUITE_END();