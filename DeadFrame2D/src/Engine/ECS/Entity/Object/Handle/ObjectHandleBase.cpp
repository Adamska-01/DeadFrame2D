#include "Engine/ECS/Entity/Object/Handle/ObjectHandleBase.h"
#include "Engine/ECS/System/Scene/Abstractions/ISceneHandleProvider.h"


namespace DeadFrame2D::Engine
{
	ObjectHandleBase::ObjectHandleBase(std::weak_ptr<ISceneHandleProvider> scene, uint32_t index, uint32_t generation)
		: scene(std::move(scene)), index(index), generation(generation)
	{
	}

	ObjectHandleBase::ObjectHandleBase(const ObjectHandleBase& other)
		: scene(other.scene), index(other.index), generation(other.generation)
	{
	}

	bool ObjectHandleBase::IsValid() const noexcept
	{
		auto lockedScene = scene.lock();

		if (!lockedScene)
			return false;

		return lockedScene->IsValid(index, generation);
	}


	bool ObjectHandleBase::operator==(const ObjectHandleBase& other) const noexcept
	{
		return scene.lock() == other.scene.lock()
			&& index == other.index
			&& generation == other.generation;
	}

	bool ObjectHandleBase::operator!=(const ObjectHandleBase& other) const noexcept
	{
		return !(*this == other);
	}

	bool ObjectHandleBase::operator==(std::nullptr_t) const noexcept
	{
		return !IsValid();
	}

	bool ObjectHandleBase::operator!=(std::nullptr_t) const noexcept
	{
		return IsValid();
	}

	ObjectHandleBase::operator bool() const noexcept
	{
		return IsValid();
	}

	ObjectHandleBase& ObjectHandleBase::operator=(std::nullptr_t)
	{
		scene.reset();
		index = UINT32_MAX;
		generation = 0;

		return *this;
	}

	const ISceneHandleProvider* ObjectHandleBase::GetSceneHandleProvider() const noexcept
	{
		return scene.expired() ? nullptr : scene.lock().get();
	}

	uint32_t ObjectHandleBase::GetIndex() const noexcept
	{
		return index;
	}

	uint32_t ObjectHandleBase::GetGeneration() const noexcept
	{
		return generation;
	}
}