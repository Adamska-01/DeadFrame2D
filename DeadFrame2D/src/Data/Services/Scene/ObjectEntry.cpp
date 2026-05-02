#include "Data/Services/Scene/ObjectEntry.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"


namespace DF2D::Data
{
	// cpp is necessary to avoid circular dependency issue between Scene and GameObject
	ObjectEntry::ObjectEntry() = default;

	ObjectEntry::~ObjectEntry() = default;
}