#include "Engine/Entity/ComponentHandleBase.h"


DeadFrame2D::Engine::ComponentHandleBase::ComponentHandleBase(std::weak_ptr<ComponentBucket> bucket, uint32_t index, uint32_t generation)
	: bucket(std::move(bucket)), 
	index(index), 
	generation(generation)
{
}

DeadFrame2D::Engine::ComponentHandleBase::ComponentHandleBase(const ComponentHandleBase& other)
	: bucket(other.bucket), 
	index(other.index), 
	generation(other.generation)
{
}