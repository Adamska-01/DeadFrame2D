#include "Constants/MathConstants.h"
#include "Engine/ECS/Component/Transform.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Constants;
using namespace DF2D::Core;
using namespace DF2D::Engine;


namespace
{
	struct TransformHierarchyFixture
	{
		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> parentOwner;

		ObjectHandle<TestGameObject> childOwner;


		TransformHierarchyFixture()
		{
			scene = std::make_shared<FakeSceneHandleProvider>();

			parentOwner = scene->Create<TestGameObject>();
			childOwner = scene->Create<TestGameObject>();

			childOwner->SetParent(parentOwner);
		}
	};
}


TEST_SUITE_BEGIN("Transform");


TEST_CASE("Constructor makes the local values available as world values without any prior mutator call")
{
	Transform t(Vector2F{ 5.0f, 10.0f }, Vector2F{ 2.0f, 3.0f }, 45.0f);

	CHECK(t.GetWorldPosition() == Vector2F{ 5.0f, 10.0f });
	CHECK(t.GetWorldScale() == Vector2F{ 2.0f, 3.0f });
	CHECK(t.GetWorldRotation() == doctest::Approx(45.0f));
}

TEST_CASE("Default constructor yields identity world transform")
{
	Transform t;

	CHECK(t.GetWorldPosition() == Vector2F::Zero);
	CHECK(t.GetWorldScale() == Vector2F::One);
	CHECK(t.GetWorldRotation() == doctest::Approx(0.0f));
}

TEST_CASE("Update captures the current world position without requiring a prior GetWorldPosition call")
{
	Transform t(Vector2F{ 3.0f, -4.0f });

	t.Update(0.016f);

	CHECK(t.GetStartFramePosition() == Vector2F{ 3.0f, -4.0f });
}

TEST_CASE("GetLocalPosition/Scale/Rotation always return the raw local values")
{
	Transform t;

	t.SetLocalPosition(Vector2F{ 1.0f, 2.0f });
	t.SetLocalScale(Vector2F{ 3.0f, 4.0f });
	t.SetLocalRotation(20.0f);

	CHECK(t.GetLocalPosition() == Vector2F{ 1.0f, 2.0f });
	CHECK(t.GetLocalScale() == Vector2F{ 3.0f, 4.0f });
	CHECK(t.GetLocalRotation() == doctest::Approx(20.0f));
}

TEST_CASE("TranslateX/TranslateY/Translate accumulate onto local position")
{
	Transform t;

	t.TranslateX(5.0f);
	t.TranslateY(-2.0f);

	CHECK(t.GetWorldPosition() == Vector2F{ 5.0f, -2.0f });

	t.Translate(Vector2F{ 1.0f, 1.0f });

	CHECK(t.GetWorldPosition() == Vector2F{ 6.0f, -1.0f });
}

TEST_CASE("ScaleX/ScaleY set individual axes without touching the other")
{
	Transform t;

	t.ScaleX(2.0f);

	CHECK(t.GetWorldScale() == Vector2F{ 2.0f, 1.0f });

	t.ScaleY(3.0f);

	CHECK(t.GetWorldScale() == Vector2F{ 2.0f, 3.0f });
}

TEST_CASE("Scale overwrites both axes at once")
{
	Transform t;

	t.ScaleX(2.0f);
	t.Scale(Vector2F{ 5.0f, 5.0f });

	CHECK(t.GetWorldScale() == Vector2F{ 5.0f, 5.0f });
}

TEST_CASE("RotateByDegrees accumulates local rotation in degrees")
{
	Transform t;

	t.RotateByDegrees(90.0f);

	CHECK(t.GetWorldRotation() == doctest::Approx(90.0f));

	t.RotateByDegrees(45.0f);

	CHECK(t.GetWorldRotation() == doctest::Approx(135.0f));
}

TEST_CASE("RotateByRadians converts to degrees before accumulating")
{
	Transform t;

	t.RotateByRadians(MathConstants::PI_f); // 180 degrees

	CHECK(t.GetWorldRotation() == doctest::Approx(180.0f));
}

TEST_CASE("GetForward returns the unit vector for the current world rotation")
{
	Transform t;

	auto forward0 = t.GetForward();

	CHECK(forward0.x == doctest::Approx(1.0f));
	CHECK(forward0.y == doctest::Approx(0.0f).epsilon(0.0001));

	t.RotateByDegrees(90.0f);

	auto forward90 = t.GetForward();

	CHECK(forward90.x == doctest::Approx(0.0f).epsilon(0.0001));
	CHECK(forward90.y == doctest::Approx(1.0f));
}

TEST_CASE("SetLocalPosition/Scale/Rotation overwrite the corresponding local value")
{
	Transform t(Vector2F{ 1.0f, 1.0f }, Vector2F{ 2.0f, 2.0f }, 10.0f);

	t.SetLocalPosition(Vector2F{ 9.0f, 9.0f });
	t.SetLocalScale(Vector2F{ 4.0f, 4.0f });
	t.SetLocalRotation(20.0f);

	CHECK(t.GetWorldPosition() == Vector2F{ 9.0f, 9.0f });
	CHECK(t.GetWorldScale() == Vector2F{ 4.0f, 4.0f });
	CHECK(t.GetWorldRotation() == doctest::Approx(20.0f));
}


TEST_CASE_FIXTURE(TransformHierarchyFixture, "GetWorldPosition composes the parent's world position, rotation and scale")
{
	parentOwner->GetTransform()->SetLocalPosition(Vector2F{ 10.0f, 0.0f });
	parentOwner->GetTransform()->SetLocalRotation(90.0f);
	parentOwner->GetTransform()->SetLocalScale(Vector2F{ 2.0f, 2.0f });

	childOwner->GetTransform()->SetLocalPosition(Vector2F{ 1.0f, 0.0f });

	// Child's local (1,0) is scaled by 2 -> (2,0), rotated 90deg -> (0,2), then offset by parent's world pos (10,0)
	auto worldPos = childOwner->GetTransform()->GetWorldPosition();

	CHECK(worldPos.x == doctest::Approx(10.0f).epsilon(0.001));
	CHECK(worldPos.y == doctest::Approx(2.0f).epsilon(0.001));

	CHECK(childOwner->GetTransform()->GetWorldScale() == Vector2F{ 2.0f, 2.0f });
	CHECK(childOwner->GetTransform()->GetWorldRotation() == doctest::Approx(90.0f));
}

TEST_CASE_FIXTURE(TransformHierarchyFixture, "Moving the parent invalidates the child's cached world position")
{
	auto initialChildWorldPos = childOwner->GetTransform()->GetWorldPosition();

	CHECK(initialChildWorldPos == Vector2F::Zero);

	parentOwner->GetTransform()->TranslateX(50.0f);

	CHECK(childOwner->GetTransform()->GetWorldPosition() == Vector2F{ 50.0f, 0.0f });
}

TEST_CASE_FIXTURE(TransformHierarchyFixture, "SetWorldPosition converts into the correct local position given the parent transform")
{
	parentOwner->GetTransform()->SetLocalPosition(Vector2F{ 10.0f, 10.0f });

	childOwner->GetTransform()->SetWorldPosition(Vector2F{ 15.0f, 10.0f });

	CHECK(childOwner->GetTransform()->GetLocalPosition() == Vector2F{ 5.0f, 0.0f });
	CHECK(childOwner->GetTransform()->GetWorldPosition() == Vector2F{ 15.0f, 10.0f });
}

TEST_CASE_FIXTURE(TransformHierarchyFixture, "SetWorldScale converts into the correct local scale given the parent transform")
{
	parentOwner->GetTransform()->SetLocalScale(Vector2F{ 2.0f, 4.0f });

	childOwner->GetTransform()->SetWorldScale(Vector2F{ 10.0f, 8.0f });

	CHECK(childOwner->GetTransform()->GetLocalScale() == Vector2F{ 5.0f, 2.0f });
	CHECK(childOwner->GetTransform()->GetWorldScale() == Vector2F{ 10.0f, 8.0f });
}

TEST_CASE_FIXTURE(TransformHierarchyFixture, "SetWorldRotation converts into the correct local rotation given the parent transform")
{
	parentOwner->GetTransform()->SetLocalRotation(30.0f);

	childOwner->GetTransform()->SetWorldRotation(100.0f);

	CHECK(childOwner->GetTransform()->GetLocalRotation() == doctest::Approx(70.0f));
	CHECK(childOwner->GetTransform()->GetWorldRotation() == doctest::Approx(100.0f));
}

TEST_CASE_FIXTURE(TransformHierarchyFixture, "Set*Position/Scale/Rotation on a root transform behave the same for local and world")
{
	parentOwner->GetTransform()->SetWorldPosition(Vector2F{ 7.0f, -3.0f });
	parentOwner->GetTransform()->SetWorldScale(Vector2F{ 2.0f, 2.0f });
	parentOwner->GetTransform()->SetWorldRotation(15.0f);

	CHECK(parentOwner->GetTransform()->GetLocalPosition() == Vector2F{ 7.0f, -3.0f });
	CHECK(parentOwner->GetTransform()->GetLocalScale() == Vector2F{ 2.0f, 2.0f });
	CHECK(parentOwner->GetTransform()->GetLocalRotation() == doctest::Approx(15.0f));
}


TEST_SUITE_END();