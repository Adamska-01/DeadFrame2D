#include <ComponentBucket.h>


size_t ComponentBucket::GetComponentID() const
{
	static size_t lastID = 0;

	return lastID++;
}

ComponentBucket::~ComponentBucket()
{
	components.clear();
}