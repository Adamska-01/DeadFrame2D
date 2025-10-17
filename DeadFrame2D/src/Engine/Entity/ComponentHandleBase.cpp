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
}