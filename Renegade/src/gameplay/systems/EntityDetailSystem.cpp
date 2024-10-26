#include "gameplay/systems/EntityDetailSystem.h"

#define JSON_ENTITY_DETAIL_COMPONENT_VAR "details"

namespace renegade
{
	namespace gameplay
	{
		std::string EntityDetailSystem::GetPropertyName() const
		{
			return JSON_ENTITY_DETAIL_COMPONENT_VAR;
		}
	}
}