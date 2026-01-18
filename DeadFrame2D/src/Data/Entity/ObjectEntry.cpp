#include "Data/Entity/ObjectEntry.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Data
{
	// cpp is necessary to avoid circular dependency issue between Scene and GameObject
	ObjectEntry::ObjectEntry() = default;

	ObjectEntry::~ObjectEntry() = default;
}