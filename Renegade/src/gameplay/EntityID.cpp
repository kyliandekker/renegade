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

		TransformComponent* EntityID::transform() const
		{
			TransformSystem& tSys = core::ENGINE.GetECS().GetSystem<TransformSystem>();
			return tSys.GetComponent(*this);
		}

		const std::string EntityID::GetName() const
		{
			return m_Name;
		}

		void EntityID::SetName(const std::string& a_Name)
		{
			m_Name = a_Name;
		}

		bool EntityID::IsActive() const
		{
			return m_Active;
		}

		void EntityID::SetActive(bool a_Active)
		{
			m_Active = a_Active;
		}
	}
}