#include "gameplay/systems/EntityDetailComponent.h"

#include <rapidjson/utils.h>

#define JSON_ENTITY_DETAIL_COMPONENT_ACTIVE_VAR "active"
#define JSON_ENTITY_DETAIL_COMPONENT_NAME_VAR "name"

namespace renegade
{
	namespace gameplay
	{
        const std::string EntityDetailComponent::GetName() const
		{
			return m_Name;
		}

		void EntityDetailComponent::SetName(const std::string& a_Name)
		{
			m_Name = a_Name;
		}

		bool EntityDetailComponent::IsActive() const
		{
			return m_Active;
		}

		void EntityDetailComponent::SetActive(bool a_Active)
		{
			m_Active = a_Active;
		}

		EntityID* EntityDetailComponent::GetParent() const
		{
			return m_Parent;
		}

		void EntityDetailComponent::SetParent(EntityID& a_EntityID)
		{
			m_Parent = &a_EntityID;
		}

		void EntityDetailComponent::Serialize(rapidjson::Value& a_Document, rapidjson::Document::AllocatorType& a_Allocator) const
		{
			if (!a_Document.IsObject())
			{
				return;
			}

			a_Document.AddMember(JSON_ENTITY_DETAIL_COMPONENT_ACTIVE_VAR, m_Active, a_Allocator);
			a_Document.AddMember(JSON_ENTITY_DETAIL_COMPONENT_NAME_VAR, rapidjson::Value().SetString(m_Name.c_str(), m_Name.size()), a_Allocator);
		}

		void EntityDetailComponent::Deserialize(const rapidjson::Value& a_Document, rapidjson::Document::AllocatorType& a_Allocator)
		{
			if (!a_Document.IsObject())
			{
				return;
			}

			if (a_Document.HasMember(JSON_ENTITY_DETAIL_COMPONENT_ACTIVE_VAR) && a_Document[JSON_ENTITY_DETAIL_COMPONENT_ACTIVE_VAR].IsBool())
			{
				rapidjson::GetBool(a_Document, JSON_ENTITY_DETAIL_COMPONENT_ACTIVE_VAR, m_Active);
			}
			if (a_Document.HasMember(JSON_ENTITY_DETAIL_COMPONENT_NAME_VAR) && a_Document[JSON_ENTITY_DETAIL_COMPONENT_NAME_VAR].IsString())
			{
				m_Name = a_Document[JSON_ENTITY_DETAIL_COMPONENT_NAME_VAR].GetString();
			}
		}
	}
}