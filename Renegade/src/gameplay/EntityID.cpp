#include "gameplay/EntityID.h"

#include "gameplay/systems/TransformComponent.h"
#include "gameplay/systems/TransformSystem.h"
#include "core/Engine.h"

namespace renegade
{
	namespace gameplay
	{
		bool EntityID::operator==(const EntityID& a_Other) const
		{
			return m_ID == a_Other.m_ID;
		}

		bool EntityID::operator!=(const EntityID& a_Other) const
		{
			return m_ID != a_Other.m_ID;
		}

		bool EntityID::operator<(const EntityID& a_Other) const
		{
			return m_ID < a_Other.m_ID;
		}
	}
}