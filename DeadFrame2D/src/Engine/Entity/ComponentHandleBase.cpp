#include "Engine/Entity/ComponentBucket.h"
#include "Engine/Entity/ComponentHandleBase.h"


namespace DeadFrame2D::Engine
{
	ComponentHandleBase::ComponentHandleBase(std::weak_ptr<ComponentBucket> bucket, uint32_t index, uint32_t generation)
		: bucket(std::move(bucket)), 
		index(index), 
		generation(generation)
	{
	}

	ComponentHandleBase::ComponentHandleBase(const ComponentHandleBase& other)
		: bucket(other.bucket), 
		index(other.index), 
		generation(other.generation)
	{
	}


	bool ComponentHandleBase::IsValid() const
	{
		auto locked = bucket.lock();

		return locked != nullptr ? locked->IsValid(index, generation) : false;
	}


	bool ComponentHandleBase::operator==(const ComponentHandleBase& other) const
	{
		return bucket.lock() == other.bucket.lock()
			&& index == other.index
			&& generation == other.generation;
	}

	bool ComponentHandleBase::operator!=(const ComponentHandleBase& other) const
	{
		return !(*this == other);
	}

	bool ComponentHandleBase::operator==(std::nullptr_t) const
	{
		return !IsValid();
	}

	bool ComponentHandleBase::operator!=(std::nullptr_t) const
	{
		return IsValid();
	}

	ComponentHandleBase::operator bool() const
	{
		return IsValid();
	}

	ComponentHandleBase& ComponentHandleBase::operator=(std::nullptr_t)
	{
		bucket.reset();
		index = UINT32_MAX;
		generation = 0;

		return *this;
	}
}