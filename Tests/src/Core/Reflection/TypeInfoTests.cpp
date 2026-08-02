#include "Core/Reflection/TypeInfo.h"
#include "Core/Reflection/TypeInfoCheck.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include <cstring>
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Engine;


// Minimal standalone hierarchy exercising the TYPE_INFO macro outside the ECS
class FakeReflectionRoot
{
public:
	static inline const TypeInfo StaticTypeInfo{ "FakeReflectionRoot", nullptr };


	virtual ~FakeReflectionRoot() = default;


	virtual const TypeInfo* GetTypeInfo() const
	{
		return &StaticTypeInfo;
	}
};


class FakeReflectionChild : public FakeReflectionRoot
{
	TYPE_INFO(FakeReflectionChild, FakeReflectionRoot);

public:
	FakeReflectionChild() = default;
};


class FakeReflectionGrandchild : public FakeReflectionChild
{
	TYPE_INFO(FakeReflectionGrandchild, FakeReflectionChild);

public:
	FakeReflectionGrandchild() = default;
};


// GameComponent-derived hierarchy for exercising the IsA parent-chain walk
class ReflectedComponent : public GameComponent
{
	TYPE_INFO(ReflectedComponent, GameComponent);

public:
	ReflectedComponent() = default;
};


class ReflectedComponentChild : public ReflectedComponent
{
	TYPE_INFO(ReflectedComponentChild, ReflectedComponent);

public:
	ReflectedComponentChild() = default;
};


class UnrelatedComponent : public GameComponent
{
	TYPE_INFO(UnrelatedComponent, GameComponent);

public:
	UnrelatedComponent() = default;
};


struct PlainStruct
{
	int value = 0;
};


TEST_SUITE_BEGIN("TypeInfo");


TEST_CASE("TYPE_INFO stores the type name")
{
	CHECK(std::strcmp(FakeReflectionChild::StaticTypeInfo.name, "FakeReflectionChild") == 0);
	CHECK(std::strcmp(FakeReflectionGrandchild::StaticTypeInfo.name, "FakeReflectionGrandchild") == 0);
}


TEST_CASE("TYPE_INFO links the parent chain")
{
	CHECK(FakeReflectionRoot::StaticTypeInfo.parent == nullptr);
	CHECK(FakeReflectionChild::StaticTypeInfo.parent == &FakeReflectionRoot::StaticTypeInfo);
	CHECK(FakeReflectionGrandchild::StaticTypeInfo.parent == &FakeReflectionChild::StaticTypeInfo);
}


TEST_CASE("Each type has a distinct TypeInfo identity")
{
	CHECK(&FakeReflectionRoot::StaticTypeInfo != &FakeReflectionChild::StaticTypeInfo);
	CHECK(&FakeReflectionChild::StaticTypeInfo != &FakeReflectionGrandchild::StaticTypeInfo);
}


TEST_CASE("GetTypeInfo dispatches virtually to the most derived type")
{
	FakeReflectionGrandchild grandchild;

	const FakeReflectionRoot* asRoot = &grandchild;

	CHECK(asRoot->GetTypeInfo() == &FakeReflectionGrandchild::StaticTypeInfo);

	FakeReflectionChild child;

	const FakeReflectionRoot* childAsRoot = &child;

	CHECK(childAsRoot->GetTypeInfo() == &FakeReflectionChild::StaticTypeInfo);
}


TEST_CASE("IsA walks the parent chain upwards")
{
	ReflectedComponentChild component;

	CHECK(component.IsA(&ReflectedComponentChild::StaticTypeInfo));
	CHECK(component.IsA(&ReflectedComponent::StaticTypeInfo));
	CHECK(component.IsA(&GameComponent::StaticTypeInfo));
}


TEST_CASE("IsA rejects unrelated types and never walks downwards")
{
	ReflectedComponentChild component;
	UnrelatedComponent unrelated;

	// Sibling branch of the hierarchy
	CHECK_FALSE(component.IsA(&UnrelatedComponent::StaticTypeInfo));

	// A base instance is not its derived type
	CHECK_FALSE(unrelated.IsA(&ReflectedComponentChild::StaticTypeInfo));

	ReflectedComponent base;

	CHECK_FALSE(base.IsA(&ReflectedComponentChild::StaticTypeInfo));
}


TEST_CASE("HasTypeInfo detects the StaticTypeInfo member")
{
	static_assert(HasTypeInfo<FakeReflectionChild>::value, "TYPE_INFO types must satisfy HasTypeInfo");
	static_assert(HasTypeInfo<GameComponent>::value, "GameComponent must satisfy HasTypeInfo");
	static_assert(!HasTypeInfo<PlainStruct>::value, "plain structs must not satisfy HasTypeInfo");
	static_assert(!HasTypeInfo<int>::value, "primitives must not satisfy HasTypeInfo");

	CHECK(HasTypeInfo<FakeReflectionGrandchild>::value);
	CHECK(HasTypeInfo<ReflectedComponentChild>::value);
	CHECK_FALSE(HasTypeInfo<PlainStruct>::value);
}


TEST_SUITE_END();