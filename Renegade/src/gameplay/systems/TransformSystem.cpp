#include "gameplay/systems/TransformSystem.h"

#define JSON_ENTITY_TRANSFORM_COMPONENT_VAR "transform"

namespace renegade
{
	namespace gameplay
	{
		std::string TransformSystem::GetPropertyName() const
		{
			return JSON_ENTITY_TRANSFORM_COMPONENT_VAR;
		}
	}
}